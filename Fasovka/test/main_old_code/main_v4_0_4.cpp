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

*/

#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#include <HX711.h>  // https://wiki.iarduino.ru/page/hx_711_with_tenzo/
#include <LiquidCrystal_I2C.h>
#include <TimerMs.h>

//=============================================

GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);

#define BTN_PIN_START 2
EncButton <EB_TICK, BTN_PIN_START> btn1;
EncButton <EB_TICK, 7, 8, 9> enc; // A, B, KEY

#define DT  10                                                // Указываем номер вывода, к которому подключен вывод DT  датчика
#define SCK 11                                                // Указываем номер вывода, к которому подключен вывод SCK датчика
HX711 scale;                                                  // создаём объект scale
float unts = 0.035274;
float calibration_factor = 14.235;                            // вводим калибровочный коэффициент
unsigned long int units = 0;                                                // задаём переменную для измерений в граммах
unsigned long int ounces = 0;                                             // задаём переменную для измерений в унциях


#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

TimerMs tmr(60000, 1, 0);
TimerMs pause_ves_lcd(10, 1, 0);

//=======================================

bool flagStart = false;
int16_t forward = 260;
int16_t backward = -260;
bool flagForward = true;
bool flagBackward = false;
uint16_t pause_step = 3000; // Пауза шага двигателя в микросекундах



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
  for (int i = 0; i < 10; i ++) {                             // усредняем показания, считав значения датчика 10 раз
    units = + scale.get_units();                              // суммируем показания 10 замеров
  }
  units = units / 10;                                         // усредняем показания, разделив сумму значений на 10
  ounces = units * unts;                                      // переводим вес из унций в граммы
  if(ounces > 600) ounces = 600;
        lcd.setCursor(0, 0);
      lcd.print("Вес, грамм   ");
      lcd.setCursor(0, 1);
      static char lcd_buffer[16]; // Массив для вывода
      sprintf(lcd_buffer,"%lu", ounces); //запись в буфер текста и значений
      lcd.print(lcd_buffer); // Вывод
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
        static char lcd_buffer[16]; // Массив для вывода
        sprintf(lcd_buffer, "%d", forward); //запись в буфер текста и значений  http://rcl-radio.ru/?p=129864
        lcd.print(lcd_buffer); // Вывод
        // c	Символ
        // s	Символьная строка
        // d, i	Целое десятичное число
        // u	Целое без знаковое десятичное число
        // o	Целое восьмеричное число
        // x	Целое шестнадцатеричное число
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
  scale.begin(DT, SCK);                                       // инициируем работу с датчиком
  scale.set_scale();                                          // выполняем измерение значения без калибровочного коэффициента
  scale.tare();                                               // сбрасываем значения веса на датчике в 0
  scale.set_scale(calibration_factor);                        // устанавливаем калибровочный коэффициент
  lcd.init();
  lcd.backlight();
  tmr.setPeriodMode();
  delay(500);   
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
  // if (flagSetting && tmr.tick()) {flagSetting = false; flagLCDRedraw = true;}




}