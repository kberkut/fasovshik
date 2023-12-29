/*
Фасовочная машинка шнекового типа.
Дозирование продукта осуществляется шнеком. Вращение шнека управляется шаковым мотором NEMA17. В качестве драйвера MX1805.
Ручное управление сделано через энкодер.
Контроль веса осуществляестя мостовым датчиком веса через HX711.
Показания веса выводятся на дисплей. Подуключена библиотека с русскими символами.
Для таймеров испольуется специальная библиотека.
Измененна схемотехника, плата и двигатель запитаны через понижайки 5 В - проблема с просадкой напряжения решена.

Сделать:
+ выход и меню
+ изменение параметров в меню
+ перезапись строки пробелами в неиспользуемых ячейках

Проблемы:
- при переключении меню двигатель останавливается, возможно конфликты с библиоткой таймера.


Определение калибровочного коэффициента для мостового датчки 0,5кг
1 грамм - коэффициент 140.64
9.95 грамм - коэффициент 142.24
60.88 грамм - коэффициент 142.37
260.14 грамм - коэффициент 142.35
499.85 грамм - коэффициент 142.36


*/

#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#include <GyverHX711.h> // https://vk.com/wall-97877471_783540
#include <LiquidCrystal_I2C.h>
#include <TimerMs.h>

//=============================================

GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);

#define BTN_PIN_START 2
EncButton<EB_TICK, BTN_PIN_START> btn1;
EncButton<EB_TICK, 7, 8, 9> enc; // A, B, KEY

GyverHX711 sensor(10, 11, HX_GAIN128_A);
float unts = 0.035274;
unsigned long int units = 0;
unsigned long int ounces = 0;
int16_t scaleFactor = 143;

#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

TimerMs timerLCD_redrawWeight(1000, 1, 0);

//=======================================

bool flagStart = false;
int16_t forward = 260; // Вперед
int16_t backward = 100; // Назад
bool flagForward = true;
bool flagBackward = true;
uint16_t pause_step = 3000; // Пауза шага двигателя в микросекундах
uint16_t pause_ves = 100;   // Пауза опроса датчка веса в миллисекундах

bool flagSetting = false;
bool flagLCDRedraw = false;
bool flagBackwardOn = false;
bool flagBlockMenuSetting = false;

//=====================================

void supply()
{
  static uint32_t timer = 0;

  if (flagForward && (micros() - timer >= pause_step))
  {
    timer = micros();
    if (flagBackwardOn)
    {
      flagForward = !flagForward;
      flagBackward = !flagBackward;
    }
    stepper.setSpeedDeg(forward);
  }

  if (flagBackwardOn && flagBackward && (micros() - timer >= pause_step))
  {
    timer = micros();
    flagForward = !flagForward;
    flagBackward = !flagBackward;
    stepper.setSpeedDeg(backward);
  }
}

void weihgt()
{
  if (flagLCDRedraw)
  {
    flagLCDRedraw = false;
    lcd.clear();
  }

  static uint32_t timer = 0;
  if (sensor.available() && (millis() - timer >= pause_ves))
  {
    timer = millis();
    for (int i = 0; i < 50; i++)
      units += sensor.read() / scaleFactor; // усредняем показания
    units = units / 50;
    ounces = (units)*unts; // переводим унции в граммы
  }
}

// c	Символ
// s	Символьная строка
// d, i	Целое десятичное число
// u	Целое без знаковое десятичное число
// o	Целое восьмеричное число
// x	Целое шестнадцатеричное число

void LCD_forward()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Скорость вперед");
  lcd.setCursor(0, 1);
  static char lcd_buffer[16];         // Массив для вывода
  sprintf(lcd_buffer, "%d", forward); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
  lcd.print(lcd_buffer);              // Вывод
}

void LCD_backward()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Скорость назад");
  lcd.setCursor(0, 1);
  static char lcd_buffer[16];          // Массив для вывода
  sprintf(lcd_buffer, "%d", backward); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
  lcd.print(lcd_buffer);               // Вывод
}

void LCD_drebezg()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Дребезг");
  lcd.setCursor(0, 1);
  if (flagBackwardOn)
  {
    lcd.print("Включено");
  }
  if (!flagBackwardOn)
  {
    lcd.print("Выключено");
  }
}

void LCD_PauseStep()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Пауза шага");
  lcd.setCursor(0, 1);
  static char lcd_buffer[16];            // Массив для вывода
  sprintf(lcd_buffer, "%d", pause_step); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
  lcd.print(lcd_buffer);                 // Вывод
}

void setting()
{
  static byte menuMain = 0;
  static bool flagLCDRedrawMainMenu = false;
  if (!flagBlockMenuSetting && enc.left())
  {
    flagLCDRedrawMainMenu = true;
    menuMain = menuMain - 1;
    if (menuMain < 1)
      menuMain = 4;
  }
  if (!flagBlockMenuSetting && enc.right())
  {
    flagLCDRedrawMainMenu = true;
    menuMain = menuMain + 1;
    if (menuMain > 4)
      menuMain = 1;
  }
  switch (menuMain)
  {
  case 1:
    if (flagLCDRedrawMainMenu)
    {
      flagLCDRedrawMainMenu = false;
      LCD_forward();
    }
    if (!flagBlockMenuSetting && enc.click())
      flagBlockMenuSetting = true;
    if (flagBlockMenuSetting)
    {
      if (enc.left())
      {
        forward = forward - 10;
        LCD_forward();
      }
      if (enc.right())
      {
        forward = forward + 10;
        LCD_forward();
      }
      if (enc.click())
        flagBlockMenuSetting = false;
    }

    break;
  case 2:
    if (flagLCDRedrawMainMenu)
    {
      flagLCDRedrawMainMenu = false;
      LCD_backward();
    }
    if (!flagBlockMenuSetting && enc.click())
      flagBlockMenuSetting = true;
    if (flagBlockMenuSetting)
    {
      if (enc.left())
      {
        backward = backward - 10;
        LCD_backward();
      }
      if (enc.right())
      {
        backward = backward + 10;
        LCD_backward();
      }
      if (enc.click())
        flagBlockMenuSetting = false;
    }
    break;
  case 3:
    if (flagLCDRedrawMainMenu)
    {
      flagLCDRedrawMainMenu = false;
      LCD_drebezg();
    }
    if (!flagBlockMenuSetting && enc.click())
      flagBlockMenuSetting = true;
    if (flagBlockMenuSetting)
    {
      if (enc.left())
      {
        flagBackwardOn = false;
        LCD_drebezg();
      }
      if (enc.right())
      {
        flagBackwardOn = true;
        LCD_drebezg();
      }
      if (enc.click())
        flagBlockMenuSetting = false;
    }
    break;
  case 4:
    if (flagLCDRedrawMainMenu)
    {
      flagLCDRedrawMainMenu = false;
      LCD_PauseStep();
    }
    if (!flagBlockMenuSetting && enc.click())
      flagBlockMenuSetting = true;
    if (flagBlockMenuSetting)
    {
      if (enc.left())
      {
        pause_step = pause_step - 100;
        LCD_PauseStep();
      }
      if (enc.right())
      {
        pause_step = pause_step + 100;
        LCD_PauseStep();
      }
      if (enc.click())
        flagBlockMenuSetting = false;
    }
    break;
  default:
    break;
  }
}

void setup()
{
  // Serial.begin(9600);
  delay(500);
  sensor.tare(); // калибровка нуля
  lcd.init();
  lcd.backlight();
  timerLCD_redrawWeight.setPeriodMode();
  enc.setHoldTimeout(3000);
}

void loop()
{
  btn1.tick();
  enc.tick();

  if (flagStart && btn1.click())
  {
    flagStart = false;
    stepper.disable();
  }
  if (!flagStart && btn1.click())
    flagStart = true;
  if (flagStart)
  {
    supply();
    stepper.tick();
  }

  if (!flagSetting && enc.click())
  {
    lcd.clear();
    flagSetting = true;
    lcd.setCursor(0, 0);
    lcd.print("     Режим      ");
    lcd.setCursor(0, 1);
    lcd.print("    настроек    ");
  }
  if (flagSetting)
    setting();

  if (!flagSetting)
    weihgt();

  if (!flagSetting && timerLCD_redrawWeight.tick())
  {
    lcd.setCursor(0, 0);
    lcd.print("Вес, грамм      ");
    lcd.setCursor(0, 1);
    static char lcd_buffer[16];         // Массив для вывода
    sprintf(lcd_buffer, "%lu", ounces); // запись в буфер текста и значений
    lcd.print(lcd_buffer);              // Вывод
    lcd.print("                ");
  }

  if (flagSetting && enc.hold())
    flagSetting = false;
}