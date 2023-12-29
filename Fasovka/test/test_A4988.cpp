/*
Тест драйвера A4988
*/
#include <Arduino.h>
// крутим туда сюда, тикаем в loop

#include "GyverStepper2.h"
// GStepper2<STEPPER4WIRE> stepper(2048, 5, 3, 4, 2);
GStepper2<STEPPER2WIRE> stepper(2048, 3, 4);
bool bool_tmr1 = 1;
bool bool_tmr2 = 0;
uint32_t tmr1 = millis();
// uint32_t tmr2 = millis();

void setup()
{
  Serial.begin(9600);
  stepper.disable();
  // stepper.setMaxSpeed(150);     // скорость движения к цели
  // stepper.setAcceleration(200); // ускорение
  // stepper.setTarget(300);       // цель
  stepper.reverse(1); // инвертировать направление мотора
  // stepper.setSpeed(480);               // установить скорость в шагах/сек и запустить вращение
  pinMode(11, OUTPUT);

  digitalWrite(11, LOW);
}

void loop()
{
  stepper.tick(); // мотор асинхронно крутится тут

  // // если приехали
  // if (stepper.ready()) {
  //   dir = !dir;   // разворачиваем
  //   stepper.setTarget(dir * 300); // едем в другую сторону
  // }

  // // асинхронный вывод в порт

  if (bool_tmr1 == 1 && millis() - tmr1 >= 700)
  {
    bool_tmr1 = 0;
    bool_tmr2 = 1;
    tmr1 = millis();
    stepper.reverse(0);
    // stepper.setSpeed(480);               // установить скорость в шагах/сек и запустить вращение
    stepper.setSpeedDeg(120);
  }

  if (bool_tmr2 == 1 && millis() - tmr1 >= 300)
  {
    bool_tmr2 = 0;
    bool_tmr1 = 1;
    tmr1 = millis();
    stepper.reverse(1);
    // stepper.setSpeed(-480);               // установить скорость в шагах/сек и запустить вращение
  }
  stepper.setSpeedDeg(120);
  // stepper.setSpeed(480);               // установить скорость в шагах/сек и запустить вращение

  // if (bool_tmr1 == 1) {
  //       bool_tmr1 = 0; bool_tmr2 = 1;
  //     // stepper.reverse(0);
  // //
  //   stepper.setSpeedDeg(50);
  //   }

  // if (bool_tmr2 == 1) {
  //       bool_tmr1 = 1; bool_tmr2 = 0;
  //     // stepper.reverse(1);

  //   stepper.setSpeedDeg(15);
  // }
}