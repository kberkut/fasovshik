/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required
to interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

uint32_t timer = 0;
uint32_t test1 = 0;
uint32_t test2 = 0;

#include <LiquidCrystal_I2C.h>
#define _LCD_TYPE 1 // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
// LCD_1602_RUS lcd(0x3F, 20, 4);
LCD_1602_RUS lcd(0x27, 20, 4);

void LCD() // http://rcl-radio.ru/?p=129864
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(test1, 1);
  // lcd.setCursor(0, 1);
  // lcd.print(test2, 1); // Вывод
}

void setup()
{
  Serial.begin(57600);
  cap.begin(0x5A);

  lcd.init();
  lcd.backlight();
}

void loop()
{

  if (millis() - timer > 100)
  {
    currtouched = cap.touched();

    for (uint8_t i = 0; i < 12; i++)
    {
      // it if *is* touched and *wasnt* touched before, alert!
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)))
      {
        Serial.print(i);
        Serial.print(" touched  ");
        // Переназначим вывод
        if (i == 1)
          test1 = 1;
        if (i == 2)
          test1 = 4;
        if (i == 3)
          test1 = 7;
        if (i == 4)
          test1 = 10;
        if (i == 5)
          test1 = 2;
        if (i == 6)
          test1 = 5;
        if (i == 7)
          test1 = 8;
        if (i == 8)
          test1 = 11;
        if (i == 9)
          test1 = 3;
        if (i == 10)
          test1 = 6;
        if (i == 11)
          test1 = 9;
        if (i == 0)
          test1 = 0;
        Serial.println(test1);
        LCD();
      }
      // if it *was* touched and now *isnt*, alert!
      // if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)))
      // {
      //   Serial.print(i);
      //   test2 = i;
      //   Serial.println(" released");
      //   LCD();
      // }
    }
    lasttouched = currtouched;
  }
}
