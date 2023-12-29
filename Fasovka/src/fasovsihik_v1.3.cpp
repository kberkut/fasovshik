/*
Подключение HX711 - Коричневый - VCC; Белый - GND; Зелёный - DT; Жёлтый - SCK
v1.01 - исправлена ошибка "Обновление LCD мешает работе шаговика, он приостанавливается на время обновления экрана".
  Для этого опрос шаговика переписан по работе на прерывании. Задание вращения теперь через прерывания по совпадению.
v1.1 - переработана логика меню
v1.2 - кнопки заменены на тачпад. Переработна логика управления. Переработана логика меню. Экран заменён на двухстрочный.
v1.3 - переделаны предустановки для веса
*/

#include <Arduino.h>

volatile uint16_t counter;
volatile uint16_t compareCounter = 1; // 0.000050 c

#include <Wire.h>
#include "Adafruit_MPR121.h"
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
uint32_t timerTouchPad = 0;
uint8_t buttonNumber = 20;
bool flagOnceButton = false;

#include <GyverHX711.h>
#define PIN_DT 8
#define PIN_SCK 7
GyverHX711 sensor(PIN_DT, PIN_SCK, HX_GAIN128_A); // DT - 8; SCK - 7
float scaleFactor = 4037.46;
float weight = 0;
uint32_t timerWeight = 0;

#include <LiquidCrystal_I2C.h>
#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
// LCD_1602_RUS lcd(0x3F, 20, 4);
// LCD_1602_RUS lcd(0x27, 20, 4);
LCD_1602_RUS lcd(0x3F, 16, 2);

// Настройки шагового двигателя
#include "GyverStepper2.h"
#define STEPPER_PIN_STEP 3
#define STEPPER_PIN_DIR 4
GStepper2<STEPPER2WIRE> stepper(2048, 3, 4);
#define STEPPER_PIN_START 2 // EN
bool flagTimerStepper1 = 1;
bool flagTimerStepper2 = 0;
uint32_t timerStepper = millis();
uint32_t timerStepperTick = 0;

// Общие установки
uint32_t timerLCDredraw = 0;
int8_t numberSetting = 0;
int8_t numberSettingMax = 1;

int8_t numberSettingWeight = 0;

int8_t granulaWeight[8] = {35, 45, 46, 50, 90, 95, 100, 115};
int8_t listWeight[5] = {52, 55, 67, 70, 75};
int8_t numberSettingWeightMax;

bool flagStart = false;
bool flagEditWeight = false;
int8_t stepWeight = 0;
bool menuTime = true; // Включение учёта времени фасовки в ёмкость
uint16_t weightTime = 0;
uint64_t weightTimeStart = 0;

// Предустановки для веса
int16_t weightSetting;
String textSetting = "";

// Настройки мотора
bool flagDirection = true;
bool flagOnce = false;
uint32_t timerMotor = 0;
uint16_t pauseMotor = 1500;
#define MOTOR_PIN_IN1 5
#define MOTOR_PIN_IN2 6

void selectMode()
{
  switch (numberSetting)
  {
  case 0:
    textSetting = "Лист:";
    break;

  case 1:
    textSetting = "Гранула:";
    break;

  default:
    break;
  }
}

void selectWeight()
{
  if (numberSetting == 0)
  {
    numberSettingWeightMax = sizeof(listWeight) - 1;
    weightSetting = listWeight[numberSettingWeight];
  }
  if (numberSetting == 1)
  {
    numberSettingWeightMax = sizeof(granulaWeight) - 1;
    weightSetting = granulaWeight[numberSettingWeight];
  }
  Serial.println(numberSettingWeight);
}

// https://arduino.ru/forum/programmirovanie/vyvod-na-lcd-teksta-i-dannykh-v-odnoi-stroke
void LCD() // http://rcl-radio.ru/?p=129864
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(textSetting);
  int8_t s1;
  if (textSetting == "Лист:")
    s1 = 6;
  if (textSetting == "Гранула:")
    s1 = 9;
  lcd.setCursor(s1, 0);
  lcd.print(weightSetting, 1);
  lcd.setCursor(s1 + (((String)weightSetting).length()), 0);
  lcd.print("гр.");
  if (flagEditWeight)
  {
    lcd.setCursor(15, 0);
    lcd.print("*");
  }
  lcd.setCursor(0, 1);
  lcd.print("Вес: ");
  if (menuTime)
  {
    if (weight < 0)
      lcd.setCursor(4, 1);
    else
      lcd.setCursor(5, 1);
    lcd.print(weight, 1); // Вывод
    lcd.setCursor(12, 1);
    lcd.print(weightTime, 1); // Вывод
    lcd.setCursor(12 + (((String)weightTime).length()), 1);
    lcd.print("с"); // Вывод
  }
  else
  {
    if (weight < 0)
      lcd.setCursor(4, 1);
    else
      lcd.setCursor(5, 1);
    lcd.print(weight, 1); // Вывод
  }
}

void touchPad()
{
  currtouched = cap.touched();
  for (uint8_t i = 0; i < 12; i++)
  {
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)))
    {
      // Переназначим вывод
      if (i == 1)
      {
        buttonNumber = 1;
      }
      if (i == 2)
      {
        buttonNumber = 4;
      }
      if (i == 3)
      {
        buttonNumber = 7;
      }
      if (i == 4)
      {
        buttonNumber = 10;
      }
      if (i == 5)
      {
        buttonNumber = 2;
      }
      if (i == 6)
      {
        buttonNumber = 5;
      }
      if (i == 7)
      {
        buttonNumber = 8;
      }
      if (i == 8)
      {
        buttonNumber = 11;
      }
      if (i == 9)
      {
        buttonNumber = 3;
      }
      if (i == 10)
      {
        buttonNumber = 6;
      }
      if (i == 11)
      {
        buttonNumber = 9;
      }
      if (i == 0)
      {
        buttonNumber = 0;
      }
      flagOnceButton = true;
      // Serial.println(buttonNumber);
    }
  }
  lasttouched = currtouched;
}

void motor()
{

  if ((millis() - timerMotor >= pauseMotor))
  {
    timerMotor = millis();
    flagDirection = !flagDirection;
    flagOnce = true;
  }

  if (flagOnce == true && flagDirection == true)
  {
    flagOnce = false;
    digitalWrite(MOTOR_PIN_IN1, HIGH);
    digitalWrite(MOTOR_PIN_IN2, LOW);
  }

  if (flagOnce == true && flagDirection == false)
  {
    flagOnce = false;
    digitalWrite(MOTOR_PIN_IN1, LOW);
    digitalWrite(MOTOR_PIN_IN2, HIGH);
  }
}

void stepperMotor()
{
  static int16_t speed = 30;
  if (flagStart)
  {
    if (flagTimerStepper1 == 1 && millis() - timerStepper >= 800) // 700  700
    {
      flagTimerStepper1 = 0;
      flagTimerStepper2 = 1;
      timerStepper = millis();
      stepper.reverse(0);
      stepper.setSpeedDeg(speed);
    }

    if (flagTimerStepper2 == 1 && millis() - timerStepper >= 100) // 200  100
    {
      flagTimerStepper2 = 0;
      flagTimerStepper1 = 1;
      timerStepper = millis();
      stepper.reverse(1);
      stepper.setSpeedDeg(speed);
    }
  }
}

void setup()
{
  Serial.begin(57600);
  // инициализация Timer1
  cli();      // отключить глобальные прерывания
  TCCR1A = 0; // установить TCCR2A регистр в 0
  TCCR1B = 0;
  OCR1A = 11;              // установка регистра совпадения
  TCCR1B |= (1 << WGM12);  // включить CTC режим
  TCCR1B |= (1 << CS10);   // Устанавливаем делитель на 64
  TCCR1B |= (1 << CS11);   //
  TCCR1B |= (0 << CS12);   //
  TIMSK1 |= (1 << OCIE1A); // включить прерывание по совпадению таймера
  sei();                   // включить глобальные прерывания

  pinMode(MOTOR_PIN_IN1, OUTPUT);
  pinMode(MOTOR_PIN_IN2, OUTPUT);

  pinMode(STEPPER_PIN_START, OUTPUT);
  digitalWrite(STEPPER_PIN_START, HIGH);
  // stepper.reverse(0);

  lcd.init();
  lcd.backlight();
  cap.begin(0x5A); // Ининциализация тачпада MPR121
  // нужно выждать готовность датчика для тарирования при первом запуске
  delay(500);
  sensor.tare(); // калибровка нуля
  numberSetting = 0;
  selectMode();
  selectWeight();
}

ISR(TIMER1_COMPA_vect) // Прерывание по совпадению
{
  stepper.tick();
}

void loop()
{
  // Опрос датчика веса
  if (sensor.available() && (millis() - timerWeight >= 100))
  {
    timerWeight = millis();
    weight = sensor.read() / scaleFactor;
  }

  // Обновление экрана постоянное
  if (millis() - timerLCDredraw >= 500)
  {
    timerLCDredraw = millis();
    LCD();
  }

  // Презапись регистров таймера для регулирования скорости шаговика
  if (flagStart && weight < weightSetting)
  {
    if (stepWeight == 0 && (weight >= weightSetting * 0.8))
    {
      cli();       // отключить глобальные прерывания
      OCR1A = 124; // установка регистра совпадения
      sei();       // включить глобальные прерывания
      stepWeight = 1;
    }
    if (stepWeight == 1 && (weight >= weightSetting * 0.9))
    {
      cli();        // отключить глобальные прерывания
      OCR1A = 1249; // установка регистра совпадения
      sei();        // включить глобальные прерывания
      stepWeight = 2;
    }
    if (stepWeight == 2 && (weight >= weightSetting * 0.95))
    {
      cli();         // отключить глобальные прерывания
      OCR1A = 12499; // установка регистра совпадения
      sei();         // включить глобальные прерывания
      stepWeight = 3;
    }
    if (stepWeight == 3 && (weight >= weightSetting * 0.98))
    {
      cli();         // отключить глобальные прерывания
      OCR1A = 24999; // установка регистра совпадения
      sei();         // включить глобальные прерывания
    }
  }

  // Автоматическая остановка дозатора при достижении утановленного веса
  if (flagStart && weight >= weightSetting)
  {
    flagStart = false;
    digitalWrite(MOTOR_PIN_IN1, LOW);
    digitalWrite(MOTOR_PIN_IN2, LOW);
    digitalWrite(STEPPER_PIN_START, HIGH);
    if (menuTime)
    {
      weightTime = (millis() - weightTimeStart) / 1000;
    }
    LCD();
  }

  // Тарирование весов ручное
  if (!flagStart && !flagEditWeight && flagOnceButton && buttonNumber == 10)
  {
    flagOnceButton = false;
    sensor.tare();
    LCD();
  }

  // Переключение режима работы дозатора в меню
  if (flagOnceButton && !flagEditWeight && buttonNumber == 7)
  {
    flagOnceButton = false;
    numberSetting++;
    if (numberSetting > numberSettingMax)
      numberSetting = 0;
    selectMode();
    selectWeight();
    LCD();
  }

  // Переключение режима работы дозатора в меню
  if (flagOnceButton && !flagEditWeight && buttonNumber == 4)
  {
    flagOnceButton = false;
    numberSetting--;
    if (numberSetting < 0)
      numberSetting = numberSettingMax;
    selectMode();
    selectWeight();
    LCD();
  }

  // Переключение предустановок веса
  if (flagOnceButton && !flagEditWeight && buttonNumber == 8)
  {
    flagOnceButton = false;
    numberSettingWeight++;
    if (numberSettingWeight > numberSettingWeightMax)
      numberSettingWeight = 0;
    selectWeight();
    LCD();
  }

  // Переключение предустановок веса
  if (flagOnceButton && !flagEditWeight && buttonNumber == 5)
  {
    flagOnceButton = false;
    numberSettingWeight--;
    if (numberSettingWeight < 0)
      numberSettingWeight = numberSettingWeightMax;
    selectWeight();
    LCD();
  }

  // Запуск работы дозатора - при нажатии кнопки пуск
  if (!flagStart && !flagEditWeight && flagOnceButton && buttonNumber == 0)
  {
    flagOnceButton = false;
    flagStart = true;
    stepWeight = 0;
    cli();      // отключить глобальные прерывания
    OCR1A = 11; // установка регистра совпадения
    sei();      // включить глобальные прерывания
    digitalWrite(STEPPER_PIN_START, LOW);
    if (menuTime)
    {
      weightTime = 0;
      weightTimeStart = millis();
    }
  }

  // Запуск выгрузки продукта - при нажатии кнопки сброс
  if (!flagStart && !flagEditWeight && flagOnceButton && buttonNumber == 2)
  {
    flagOnceButton = false;
    flagStart = true;
    stepWeight = 0;
    weightSetting = 1000;
    cli();      // отключить глобальные прерывания
    OCR1A = 11; // установка регистра совпадения
    sei();      // включить глобальные прерывания
    digitalWrite(STEPPER_PIN_START, LOW);
  }

  // Остановка работы дозатора - при нажатии кнопки стоп
  if (flagStart && !flagEditWeight && flagOnceButton && buttonNumber == 1)
  {
    flagOnceButton = false;
    flagStart = false;
    digitalWrite(STEPPER_PIN_START, HIGH);
    digitalWrite(MOTOR_PIN_IN1, LOW);
    digitalWrite(MOTOR_PIN_IN2, LOW);
    LCD();
  }

  // Запуск шаговика и мотора смешивателя при запуске дозатора
  if (flagStart)
  {
    motor();
    stepperMotor();
  }

  // Включить/выключить режим изменения веса - нажатие кнопки ввод
  if (flagOnceButton && buttonNumber == 11)
  {
    flagOnceButton = false;
    flagEditWeight = !flagEditWeight;
  }

  // Уменьшение или увеличение установки веса кнопками 7 и 9
  if (flagEditWeight == true)
  {

    if (flagOnceButton && buttonNumber == 9)
    {
      flagOnceButton = false;
      weightSetting++;
    }
    if (flagOnceButton && buttonNumber == 6)
    {
      flagOnceButton = false;
      weightSetting--;
    }
  }

  // Опрос тачпада
  if (millis() - timerTouchPad > 100)
  {
    touchPad();
  }
}