/*
Фасовочная машинка шнекового типа. 
Дозирование продукта осуществляется шнеком. Вращение шнека управляется шаковым мотором NEMA17. В качестве драйвера MX1805.
Ручное управление сделано через энкодер.
Контроль веса осуществляестя мостовым датчиком веса через HX711.
Показания веса выводятся на дисплей. Подуключена библиотека с русскими символами.
Для таймеров испольуется специальная библиотека.
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
uint32_t units = 0;
uint32_t ounces = 0;

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

TimerMs tmr(2000, 1, 0);

//=======================================

bool flagStart = false;
int16_t forward = 260;
int16_t backward = 260;
bool flagForward = true;
bool flagBackward = false;
uint16_t pause = 3000; // Пауза шага двигателя в микросекундах
uint16_t pause_ves = 100; // Пауза опроса датчка веса в миллисекундах


bool flagSetting = false;
bool flagSetSpeedForward = false;
bool flagSetSpeedBackward = false;
bool flagSetSpeedBackwardOn = false;
bool flagSetPauseStep = false;
bool flagLCDRedraw = false;

//=====================================

byte numberOfDigits(int16_t number) {
byte currentNumberOfDigits = 0;
while (number != 0) {
 number = number / 10;
 currentNumberOfDigits++;
}
return currentNumberOfDigits;
}

void supply(){
   static uint32_t timer = 0;
   
   if(flagForward && (micros() - timer >= pause)) {
    timer = micros();
    flagForward = !flagForward;
    flagBackward = !flagBackward;
   stepper.setSpeedDeg(forward);            
   }

   if(flagBackward  && (micros() - timer >= pause)){
    timer = micros();
    flagForward = !flagForward;
    flagBackward = !flagBackward;
   stepper.setSpeedDeg(backward); 
   }
}

void weihgt(){
  static uint32_t timer = 0;
  if (sensor.available() && (millis() - timer >= pause_ves)) {
     timer = millis();
for(int i = 0;i < 50; i ++) units += sensor.read();   // усредняем показания
  units = units / 50;                                                   
  ounces = units * unts;                                    // переводим унции в граммы              
    lcd.setCursor(0, 0);
    lcd.print("Вес, грамм   ");
    lcd.setCursor(0, 1);
    static char lcd_buffer[16]; // Массив для вывода
    sprintf(lcd_buffer,"%u",ounces); //запись в буфер текста и значений
    lcd.print(lcd_buffer); // Вывод
  }
}



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
        lcd.print(menuMain,1); // Вывод
      }
      break;
    case 2:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Скорость назад");
        lcd.setCursor(0, 1);
        lcd.print("Скорость-назад--"); // Вывод
      }       
      break;
    case 3:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Дребезг");
        lcd.setCursor(0, 1);


        static byte space = 16 - numberOfDigits(forward);
        static char spaceLine(0);
        static char spaceChar(32);

        while (space > 0)
        {
          spaceLine = spaceLine + spaceChar;
          space = space - 1;
        }
        static char lcd_buffer[15]; // Массив для вывода
        sprintf(lcd_buffer, "%d", forward); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
        // sprintf(lcd_buffer, "%d" PRIu32, "%d" PRId32, forward, spaceLine); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864

        lcd.print(lcd_buffer); // Вывод

        // lcd. print((String)forward + spaceLine);

        // lcd.print(menuMain,1); // Вывод
      } 


  



      break;
    case 4:
      if(flagLCDRedrawMainMenu) {
        flagLCDRedrawMainMenu = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Пауза шага");
        lcd.setCursor(0, 1);
        lcd.print(menuMain,1); // Вывод
      } 
      break;
    default:
      break;
  }

  // if(flagSetSpeedForward) {
  //   flagSetSpeedForward = false;

  //   lcd.clear();

  //   lcd.setCursor(0, 0);
  //   lcd.print("Скорость вперед");
  //   lcd.setCursor(0, 1);

  //   // static byte number = numberOfDigits(forward);
    // static byte space = 16 - numberOfDigits(forward);
    // static char spaceLine = '0';
    // char spaceChar = '32';

    // while (space > 0)
    // {
    //   spaceLine = spaceLine + spaceChar;
    //   space = space - 1;
    // }
    
  // static char lcd_buffer[16]; // Массив для вывода
  // sprintf(lcd_buffer,"%u",forward); //запись в буфер текста и значений
// lcd.print(forward,1); // Вывод


    // lcd.print((String)forward + spaceLine);
    // if(enc.right()) forward = forward + 1;
  // }

  // if(flagSetSpeedBackward) {
  //   flagSetSpeedBackward = false;

  //   lcd.clear();

  //   lcd.setCursor(0, 0);
  //   lcd.print("Скорость назад");
  //   lcd.setCursor(0, 1);
  //   static char lcd_buffer[16]; // Массив для вывода
  //   sprintf(lcd_buffer,"%u",backward); //запись в буфер текста и значений
  //   lcd.print(lcd_buffer); // Вывод
  //   // if(enc.left()) forward = forward - 1;
  // }


  // if(flagSetSpeedBackwardOn) {
  //   flagSetSpeedBackwardOn = false;

  //   lcd.clear();
  //   lcd.setCursor(0, 0);
  //   lcd.print("Дребезг");
  //   lcd.setCursor(0, 1);
  //   lcd.print("Включено");
  // }


  // if(flagSetPauseStep) {
  //   flagSetPauseStep = false;

  //   lcd.clear();

  //   lcd.setCursor(0, 0);
  //   lcd.print("Пауза шага");
  //   lcd.setCursor(0, 1);
  //   static char lcd_buffer[16]; // Массив для вывода
  //   sprintf(lcd_buffer,"%u",pause); //запись в буфер текста и значений
  //   lcd.print(lcd_buffer); // Вывод
  //   // if(enc.left()) pause = pause - 10;
  //   // if(enc.right()) pause = pause + 10;
  // }





}





void setup() {
  // Serial.begin(9600);
    delay(500);
    sensor.tare();    // калибровка нуля
    lcd.init();
    lcd.backlight();
    tmr.setPeriodMode();
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



  // if (tmr.tick()) lcd.clear();
  // if(!flagSetting) weihgt();
  if(enc.hasClicks(2)) flagSetting = false; 





}