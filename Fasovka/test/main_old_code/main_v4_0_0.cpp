#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#include <GyverHX711.h> // https://vk.com/wall-97877471_783540
#include <LiquidCrystal_I2C.h>

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x3F, 16, 2);

#include <TimerMs.h>
TimerMs tmr(2000, 1, 0);

#define BTN_PIN_START 2
#define BTN_PIN_1 7
#define BTN_PIN_2 8
#define BTN_PIN_3 9


GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);
EncButton <EB_TICK, BTN_PIN_START> btn1;
EncButton <EB_TICK, BTN_PIN_1> btn2;
EncButton <EB_TICK, BTN_PIN_2> btn3;
EncButton <EB_TICK, BTN_PIN_3> btn4;

GyverHX711 sensor(10, 11, HX_GAIN128_A);
float unts = 0.035274;
uint32_t units = 0;
uint32_t ounces = 0;

byte flagStart = false;
int16_t forward = 260;
int16_t backward = -25;
byte flagForward = true;
byte flagBackward = false;
uint16_t pause = 3000;
uint16_t pause_ves = 10000;

void driga(){
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
    lcd.setCursor(0, 4);
    lcd.print(ounces,1);
  }
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
  btn1.tick();   // дополнительный опрос таймаутов и коллбэков в loop
  btn2.tick();   // дополнительный опрос таймаутов и коллбэков в loop
  btn3.tick();   // дополнительный опрос таймаутов и коллбэков в loop
  btn4.tick();   // дополнительный опрос таймаутов и коллбэков в loop
  if(flagStart && btn1.click()) {
   flagStart = false;
   stepper.disable();
  }
  if(!flagStart && btn1.click()) flagStart = true;
  if(flagStart) {
    driga();
    stepper.tick();
  }
  if(btn2.click()) {
    forward = 26;
    backward = -24;
  }
    if(btn3.click()) {
    sensor.tare();    // калибровка нуля
  }
    weihgt();

    if(btn4.click()) {
      lcd.clear();
  }
if (tmr.tick()) lcd.clear();
}