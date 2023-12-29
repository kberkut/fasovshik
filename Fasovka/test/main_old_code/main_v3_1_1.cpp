#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#define BTN_PIN_START 2




GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);
EncButton <EB_TICK, BTN_PIN_START> btn1;

byte flagStart = false;
int8_t forward = 25;
int8_t backward = 25;
byte flagForward = true;
byte flagBackward = false;
uint16_t pause = 1000;




void work_motor(){
   static uint32_t timer = 0;
   
   if(flagForward && (micros() - timer >= pause)) {
    timer = micros();
    // flagForward = !flagForward;
    // flagBackward = !flagBackward;
 
   stepper.setSpeedDeg(forward);            // установить скорость в градусах/сек и запустить вращение

   }

   if(flagBackward  && (micros() - timer >= pause)){
    timer = micros();
    flagForward = !flagForward;
    flagBackward = !flagBackward;
   stepper.setSpeedDeg(backward); 
   }
}


void setup() {

}

void loop() {
  btn1.tick();   
  if(flagStart && btn1.hasClicks(1)) {
   flagStart = false;
   stepper.disable();
  }
  if(!flagStart && btn1.hasClicks(1)) flagStart = true;
  if(flagStart) {
    work_motor();
    stepper.tick();
  }

}