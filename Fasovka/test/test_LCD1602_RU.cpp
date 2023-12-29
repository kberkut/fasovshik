// Проверка работы дисплея с выводом русских букв
#include <LiquidCrystal_I2C.h>

#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

/*
c	Символ
s	Символьная строка
d, i	Целое десятичное число
u	Целое без знаковое десятичное число
o	Целое восьмеричное число
x	Целое шестнадцатеричное число
*/

int forward = 15;
uint32_t timer = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
}

void loop()
{

  if (millis() - 1000 >= timer)
  {
    timer = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Скорость вперед");
    lcd.setCursor(0, 1);
    static char lcd_buffer[16];         // Массив для вывода
    sprintf(lcd_buffer, "%d", forward); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
    lcd.print(lcd_buffer);              // Вывод
  }
}