/*
Фасовочная машинка шнекового типа. 
Дозирование продукта осуществляется шнеком. Вращение шнека управляется шаковым мотором NEMA17. В качестве драйвера MX1805.
Ручное управление сделано через энкодер.
Контроль веса осуществляестя мостовым датчиком веса через HX711.
Показания веса выводятся на дисплей. Подуключена библиотека с русскими символами.
Для таймеров испольуется специальная библиотека.

Сделать:
- выход и меню
- изменение параметров в меню
- перезапись строки пробелами в неиспользуемых ячейках

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
EncButton <EB_TICK, BTN_PIN_START> btn1;
EncButton <EB_TICK, 7, 8, 9> enc; // A, B, KEY

GyverHX711 sensor(10, 11, HX_GAIN128_A);
float unts = 0.035274;
unsigned long int units = 0;
unsigned long int ounces = 0;
int16_t scaleFactor = 143;


#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

TimerMs tmr(60000, 1, 0);
TimerMs tmr2(1000, 1, 0);

//=======================================

bool flagStart = false;
int16_t forward = 260;
int16_t backward = -260;
bool flagForward = true;
bool flagBackward = false;
uint16_t pause_step = 3000; // Пауза шага двигателя в микросекундах
uint16_t pause_ves = 100; // Пауза опроса датчка веса в миллисекундах
uint16_t pause_ves2 = 1000; // Пауза опроса датчка веса в миллисекундах



bool flagSetting = false;
bool flagLCDRedraw = false;

//=====================================

void supply(){
   static uint32_t timer = 0;
   
   if(flagForward && (micros() - timer >= pause_step)) {
    timer = micros();
    flagForward = !flagForward;
    flagBackward = !flagBackward;
   stepper.setSpeedDeg(forward);            
   }

   if(flagBackward  && (micros() - timer >= pause_step)){
    timer = micros();
    flagForward = !flagForward;
    flagBackward = !flagBackward;
   stepper.setSpeedDeg(backward); 
   }
}

void weihgt(){
  if(flagLCDRedraw) {
    flagLCDRedraw = false;
    lcd.clear();
  }

  static uint32_t timer = 0;
  if (sensor.available() && (millis() - timer >= pause_ves)) {
    timer = millis();
    for(int i = 0; i < 50; i ++) units += sensor.read()/scaleFactor;   // усредняем показания
    units = units / 50;                                                   
    ounces = (units) * unts;                                    // переводим унции в граммы              
  }
  
}

// c	Символ
// s	Символьная строка
// d, i	Целое десятичное число
// u	Целое без знаковое десятичное число
// o	Целое восьмеричное число
// x	Целое шестнадцатеричное число

void setting() {

  static byte menuMain = 0;
  static bool flagLCDRedrawMainMenu = false;

  if(enc.left()) {
  flagLCDRedrawMainMenu = true;
  menuMain = menuMain - 1;
  if(menuMain < 1) menuMain = 4;
  }
  if(enc.right()) {
  flagLCDRedrawMainMenu = true;
    menuMain = menuMain + 1;
  if(menuMain > 4) menuMain = 1;
  }
  switch (menuMain) {
    case 1:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Скорость вперед");
        lcd.setCursor(0, 1);
        static char lcd_buffer[16]; // Массив для вывода
        sprintf(lcd_buffer, "%d", forward); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
        lcd.print(lcd_buffer); // Вывод
      }
      break;
    case 2:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Скорость назад");
        lcd.setCursor(0, 1);
        static char lcd_buffer[16]; // Массив для вывода
        sprintf(lcd_buffer, "%d", backward); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
        lcd.print(lcd_buffer); // Вывод
      }       
      break;
    case 3:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Дребезг");
        lcd.setCursor(0, 1);
        static char lcd_buffer[16]; // Массив для вывода
        sprintf(lcd_buffer, "%d", forward); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
        lcd.print(lcd_buffer); // Вывод
      } 
      break;
    case 4:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Пауза шага");
        lcd.setCursor(0, 1);
        static char lcd_buffer[16]; // Массив для вывода
        sprintf(lcd_buffer, "%d", pause_step); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
        lcd.print(lcd_buffer); // Вывод
      } 
      break;
    default:
      break;
  }
}





void setup() {
  // Serial.begin(9600);
    delay(500);
    sensor.tare();    // калибровка нуля
    lcd.init();
    lcd.backlight();
    tmr.setPeriodMode();
    enc.setHoldTimeout(3000);
}

void loop() {
  btn1.tick();
  enc.tick();  

  if(flagStart && btn1.click()) {
    flagStart = false;
    stepper.disable();
  }
  if(!flagStart && btn1.click()) flagStart = true;
  if(flagStart) {
    supply();
    stepper.tick();
  }



  if(!flagSetting && enc.click()) {
    lcd.clear();

    flagSetting = true;
    lcd.setCursor(0, 0);
    lcd.print("Режим           ");
    lcd.setCursor(0, 1);
    lcd.print("настроек--------");
  }
  if(flagSetting) setting();



  if(!flagSetting) weihgt();

    if (!flagSetting && tmr2.tick()) {
      lcd.setCursor(0, 0);
      lcd.print("Вес, грамм      ");
      lcd.setCursor(0, 1);
      static char lcd_buffer[16]; // Массив для вывода
      sprintf(lcd_buffer,"%lu", ounces); //запись в буфер текста и значений
      lcd.print(lcd_buffer); // Вывод
      lcd.print("                ");
    }

  if(flagSetting && enc.hold()) flagSetting = false;

}