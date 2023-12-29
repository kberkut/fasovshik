#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#define BTN_PIN_START 2
#define BTN_PIN_1 7
#define BTN_PIN_2 8
#define BTN_PIN_3 9


GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);
EncButton <EB_TICK, BTN_PIN_START> btn1;
EncButton <EB_TICK, BTN_PIN_1> btn2;
EncButton <EB_TICK, BTN_PIN_2> btn3;
EncButton <EB_TICK, BTN_PIN_3> btn4;

byte flagStart = false;
int8_t forward = 26;
int8_t backward = -25;
byte flagForward = true;
byte flagBackward = false;
uint16_t pause = 3000;


// uint32_t setReverseTime = 3000;


void driga(){
   static uint32_t timer = 0;
   
   if(flagForward && (micros() - timer >= pause)) {
    timer = micros();
    // flagForward = !flagForward;
    // flagBackward = !flagBackward;
        flagForward = false;
    flagBackward = true;
   stepper.setSpeedDeg(forward);            // установить скорость в градусах/сек и запустить вращение

   }

   if(flagBackward  && (micros() - timer >= pause)){
    timer = micros();
    // flagForward = !flagForward;
    // flagBackward = !flagBackward;
           flagForward = true;
    flagBackward = false;
   stepper.setSpeedDeg(backward); 
   }
}


void setup() {


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
 forward = 50;
 backward = -38;
  }
    if(btn4.click()) {
 forward = 15;
 backward = 0;
  }
}