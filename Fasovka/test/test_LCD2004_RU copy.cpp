// Проверка работы дисплея с выводом русских букв
#include <LiquidCrystal_I2C.h>

#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
// LCD_1602_RUS lcd(0x3F, 20, 4);
LCD_1602_RUS lcd(0x27, 20, 4);

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
    lcd.print("01234567890123456789");
    lcd.setCursor(0, 2);

    static char lcd_buffer[16];                                   // Массив для вывода
    sprintf(lcd_buffer, "текст = %d = %d", forward, forward * 2); // запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
    lcd.print(lcd_buffer);                                        // Вывод
  }
}