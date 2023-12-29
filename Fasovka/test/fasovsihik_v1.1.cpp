/*
Подключение HX711 - Коричневый - VCC; Белый - GND; Зелёный - DT; Жёлтый - SCK
v1.01 - исправлена ошибка "Обновление LCD мешает работе шаговика, он приостанавливается на время обновления экрана".
  Для этого опрос шаговика переписан по работе на прерывании. Задание вращения теперь через прерывания по совпадению.
v1.1 - переработана логика меню
*/

#include <Arduino.h>

volatile uint16_t counter;
volatile uint16_t compareCounter = 1; // 0.000050 c

#include <GyverHX711.h>
#define PIN_DT 8
#define PIN_SCK 7
GyverHX711 sensor(PIN_DT, PIN_SCK, HX_GAIN128_A); // DT - 8; SCK - 7
float scaleFactor = 4036.4166;
float weight = 0;
uint32_t timerWeight = 0;

#include <LiquidCrystal_I2C.h>
#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
// LCD_1602_RUS lcd(0x3F, 20, 4);
LCD_1602_RUS lcd(0x27, 20, 4);

// Настройки кнопок управления
#include <EncButton.h>
#define BTN_PIN_TARE A0
#define BTN_PIN_UP A1
#define BTN_PIN_DOWN A3
#define BTN_PIN_ENTER A2
#define BTN_PIN_START 12
// #define BTN_PIN_STOP 2
EncButton<EB_TICK, BTN_PIN_TARE> btn1;
EncButton<EB_TICK, BTN_PIN_UP> btn2;
EncButton<EB_TICK, BTN_PIN_DOWN> btn3;
EncButton<EB_TICK, BTN_PIN_START> btn5;
EncButton<EB_TICK, BTN_PIN_ENTER> btn4;

// Настройки шагового двигателя
#include "GyverStepper2.h"
#define STEPPER_PIN_STEP 3
#define STEPPER_PIN_DIR 4
GStepper2<STEPPER2WIRE> stepper(2048, 3, 4);
#define STEPPER_PIN_START 11
bool flagTimerStepper1 = 1;
bool flagTimerStepper2 = 0;
uint32_t timerStepper = millis();
uint32_t timerStepperTick = 0;

// Общие установки
#define LED_PIN_FINISH 13
uint32_t timerLCDredraw = 0;
int16_t numberSetting = 0;
int16_t numberSettingMax = 1;
bool flagStart = false;
bool flagEditWeight = false;
int8_t stepWeight = 0;

// Предустановки для веса
int16_t weightSetting = 75;
String textSetting = "";

// Настройки мотора
bool flagDirection = true;
bool flagOnce = false;
uint32_t timerMotor = 0;
uint16_t pauseMotor = 1500;
#define MOTOR_PIN_IN1 5
#define MOTOR_PIN_IN2 6

void select()
{
  switch (numberSetting)
  {
  case 0:
    textSetting = "Лист - ";
    // weightSetting = 75;
    break;

  case 1:
    textSetting = "Гранула - ";
    // weightSetting = 100;
    break;

  default:
    break;
  }
}

// https://arduino.ru/forum/programmirovanie/vyvod-na-lcd-teksta-i-dannykh-v-odnoi-stroke

void LCD() // http://rcl-radio.ru/?p=129864
{
  lcd.clear();
  lcd.setCursor(0, 0);
  // sprintf(firstString, "%d  %d", textSetting, weightSetting);
  String firstString = String(textSetting + weightSetting + "гр.");
  lcd.print(firstString);

  // lcd.print(textSetting);
  // lcd.setCursor(12, 0);
  // lcd.print(weightSetting);
  lcd.setCursor(0, 1);
  String secondString = String("Текущий вес: ");
  lcd.print(secondString);
  lcd.setCursor(1, 2);
  lcd.print(weight, 1); // Вывод
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
  // Serial.begin(57600);

  // инициализация Timer1
  cli();      // отключить глобальные прерывания
  TCCR1A = 0; // установить TCCR2A регистр в 0
  TCCR1B = 0;

  OCR1A = 11; // установка регистра совпадения

  TCCR1B |= (1 << WGM12); // включить CTC режим

  TCCR1B |= (1 << CS10); // Устанавливаем делитель на 64
  TCCR1B |= (1 << CS11); //
  TCCR1B |= (0 << CS12); //

  TIMSK1 |= (1 << OCIE1A); // включить прерывание по совпадению таймера

  sei(); // включить глобальные прерывания

  pinMode(LED_PIN_FINISH, OUTPUT);
  digitalWrite(LED_PIN_FINISH, LOW);
  pinMode(MOTOR_PIN_IN1, OUTPUT);
  pinMode(MOTOR_PIN_IN2, OUTPUT);

  pinMode(STEPPER_PIN_START, OUTPUT);
  digitalWrite(STEPPER_PIN_START, HIGH);
  // stepper.reverse(0);

  lcd.init();
  lcd.backlight();
  // нужно выждать готовность датчика для тарирования при первом запуске
  delay(500);
  sensor.tare(); // калибровка нуля
  numberSetting = 0;
  select();
}

ISR(TIMER1_COMPA_vect) // Прерывание по совпадению
{
  stepper.tick();
}

void loop()
{
  btn1.tick();
  btn2.tick();
  btn3.tick();
  btn4.tick();
  btn5.tick();

  if (sensor.available() && (millis() - timerWeight >= 100))
  {
    timerWeight = millis();
    weight = sensor.read() / scaleFactor;
    // Serial.println(weight);
  }

  if (millis() - timerLCDredraw >= 500)
  {
    timerLCDredraw = millis();
    LCD();
  }

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

  if (flagStart && weight >= weightSetting)
  {
    digitalWrite(LED_PIN_FINISH, HIGH);
    flagStart = false;
    digitalWrite(MOTOR_PIN_IN1, LOW);
    digitalWrite(MOTOR_PIN_IN2, LOW);
    digitalWrite(STEPPER_PIN_START, HIGH);
    LCD();
  }

  if (btn1.press())
  {
    sensor.tare();
    digitalWrite(LED_PIN_FINISH, LOW);
    LCD();
  }
  if (flagEditWeight == false && btn2.press())
  {
    numberSetting++;
    if (numberSetting > numberSettingMax)
      numberSetting = 0;
    select();
    LCD();
  }

  if (flagEditWeight == false && btn3.press())
  {
    numberSetting--;

    if (numberSetting < 0)
      numberSetting = numberSettingMax;

    select();
    LCD();
  }

  if (btn5.press())
  {
    flagStart = true;
    stepWeight = 0;
    cli();      // отключить глобальные прерывания
    OCR1A = 11; // установка регистра совпадения
    sei();      // включить глобальные прерывания
    digitalWrite(STEPPER_PIN_START, LOW);
    digitalWrite(LED_PIN_FINISH, LOW);
  }

  if (flagStart)
  {
    motor();
    stepperMotor();
  }

  if (btn4.press())
  {
    flagEditWeight = !flagEditWeight;
  }

  if (flagEditWeight == true)
  {

    if (btn2.press())
      weightSetting++;
    if (btn3.press())
      weightSetting--;
  }
}