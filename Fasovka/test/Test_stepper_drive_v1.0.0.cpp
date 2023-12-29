/*
Регулировка скорости вращения шаговика
Изменение времени шага.
*/

#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#include <LiquidCrystal_I2C.h>
// #include <TimerMs.h>

//=============================================

GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);

#define BTN_PIN_START 2
EncButton<EB_TICK, BTN_PIN_START> btn1;
EncButton<EB_TICK, 7, 8, 9> enc; // A, B, KEY

#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

// TimerMs timerLCD_redrawWeight(1000, 1, 0);

//=======================================

bool flagStart = false;
int16_t forward = 180;
bool flagForward = true;
uint16_t pause_step = 3000; // Пауза шага двигателя в микросекундах
uint16_t pause_ves = 100;   // Пауза опроса датчка веса в миллисекундах

bool flagSetting = false;
bool flagLCDRedraw = true;
bool flagBackwardOn = false;
bool flagBlockMenuSetting = false;

//=====================================

void work_motor()
{
  stepper.setSpeedDeg(forward);
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

void LCD_Main()
{
  lcd.clear();
  static char lcd_buffer_speed[16];                    // Массив для вывода
  lcd.setCursor(0, 0);
  sprintf(lcd_buffer_speed, "Скорость = %d", forward); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
  static char lcd_buffer_step[16];
  lcd.setCursor(0, 1);
  sprintf(lcd_buffer_step, "Шаг = %d", pause_step); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
  lcd.print(lcd_buffer_step);                       // Вывод
}

// sprintf(aux, «T1 = %d  T2 = %d», a0, a1);

void setting()
{
  static byte menuMain = 0;
  static bool flagLCDRedrawMainMenu = false;
  flagLCDRedraw = true;
  if (!flagBlockMenuSetting && enc.left())
  {
    flagLCDRedrawMainMenu = true;
    menuMain = menuMain - 1;
    if (menuMain < 1)
      menuMain = 2;
  }
  if (!flagBlockMenuSetting && enc.right())
  {
    flagLCDRedrawMainMenu = true;
    menuMain = menuMain + 1;
    if (menuMain > 2)
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
  lcd.init();
  lcd.backlight();
  // timerLCD_redrawWeight.setPeriodMode();
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
    stepper.tick();
    work_motor();
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

  if (flagSetting && enc.hold())
    flagSetting = false;

  if (flagLCDRedraw && !flagSetting)
  {
    flagLCDRedraw = false;
    LCD_Main();
  }
}