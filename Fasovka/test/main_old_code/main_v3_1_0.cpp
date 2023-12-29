#include <Arduino.h>
#include <GyverStepper2.h>
#include <EncButton.h>
#define BTN_PIN_START 2




GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);
EncButton <EB_TICK, BTN_PIN_START> btn1;

byte flagStart = false;
int8_t forward = 50;








void setup() {

}

void loop() {
  btn1.tick();   
  if(flagStart && btn1.click()) {
   flagStart = false;
   stepper.disable();
  }
  if(!flagStart && btn1.click()) flagStart = true;
  if(flagStart) {
    stepper.tick();
    stepper.setSpeed(forward);
    
  }

}