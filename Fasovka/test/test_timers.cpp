// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#define LEDPIN 13

#include <LiquidCrystal_I2C.h>
#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
// LCD_1602_RUS lcd(0x3F, 20, 4);
LCD_1602_RUS lcd(0x27, 20, 4);

int16_t WX = 123;
String textSetting = "sdf";

void LCD()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEST");
  lcd.setCursor(1, 1);
  lcd.print(textSetting);
  lcd.setCursor(0, 2);
  lcd.print("HYS");
  lcd.setCursor(1, 3);
  lcd.print(WX, 1); // Вывод
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);

  // инициализация Timer1
  cli();      // отключить глобальные прерывания
  TCCR1A = 0; // установить TCCR1A регистр в 0
  TCCR1B = 0;

  // включить прерывание Timer1 overflow:
  TIMSK1 = (1 << TOIE1);
  // Установить CS10 бит так, чтобы таймер работал при тактовой частоте:

  TCCR1B |= (0 << CS10);
  TCCR1B |= (0 << CS11);
  TCCR1B |= (1 << CS12);

  sei(); // включить глобальные прерывания

  lcd.init();
  lcd.backlight();
}

ISR(TIMER1_OVF_vect)
{
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  LCD();
}

void loop()
{
}