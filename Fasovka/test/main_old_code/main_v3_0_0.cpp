#include <Arduino.h>
#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);
#include <EncButton.h>
#define BTN_PIN_START 2
EncButton <EB_TICK, BTN_PIN_START> btn1;
byte flagStart = false;
// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами
// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2

// создание объекта
// steps - шагов на один оборот вала (для расчётов с градусами)
// step, dir, pin1, pin2, pin3, pin4 - любые GPIO
// en - пин отключения драйвера, любой GPIO
//GStepper<STEPPER2WIRE> stepper(steps, step, dir);                   // драйвер step-dir
//GStepper<STEPPER2WIRE> stepper(steps, step, dir, en);               // драйвер step-dir + пин enable
//GStepper<STEPPER4WIRE> stepper(steps, pin1, pin2, pin3, pin4);      // драйвер 4 пин
//GStepper<STEPPER4WIRE> stepper(steps, pin1, pin2, pin3, pin4, en);  // драйвер 4 пин + enable
//GStepper<STEPPER4WIRE_HALF> stepper(steps, pin1, pin2, pin3, pin4);     // драйвер 4 пин полушаг
//GStepper<STEPPER4WIRE_HALF> stepper(steps, pin1, pin2, pin3, pin4, en); // драйвер 4 пин полушаг + enable

void setup() {
//   Serial.begin(115200);
  // режим поддержания скорости
  stepper.setRunMode(KEEP_SPEED);

  

  // можно установить скорость
  // stepper.setSpeed(300);    // в шагах/сек
  stepper.setSpeed(100);  // в градусах/сек

  // режим следования к целевй позиции
  // stepper.setRunMode(FOLLOW_POS);

  // можно установить позицию
//   stepper.setTarget(-2024);    // в шагах
  // stepper.setTargetDeg(-360);  // в градусах

  // установка макс. скорости в градусах/сек
//   stepper.setMaxSpeedDeg(400);
  
  // установка макс. скорости в шагах/сек
//   stepper.setMaxSpeed(400);

  // установка ускорения в градусах/сек/сек
//   stepper.setAccelerationDeg(300);

  // установка ускорения в шагах/сек/сек
//   stepper.setAcceleration(300);

  // отключать мотор при достижении цели
//   stepper.autoPower(true);

  // включить мотор (если указан пин en)
//   stepper.enable();
}

void loop() {
    // stepper.tick();
  // просто крутим туды-сюды
//   if (!stepper.tick()) {
//     static bool dir;
//     dir = !dir;
//     stepper.setTarget(dir ? -1024 : 1024);
//   }
  btn1.tick();   
  if(flagStart && btn1.hasClicks(1)) {
   flagStart = false;
   stepper.disable();
  }
  if(!flagStart && btn1.hasClicks(1)) flagStart = true;
  if(flagStart) {

    stepper.tick();
  }
}