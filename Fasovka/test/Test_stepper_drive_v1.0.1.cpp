/*
Регулировка скорости вращения шаговика
Изменение времени шага.
*/

#include <Arduino.h>
#include <GyverStepper2.h>

int16_t forward = 50;
//=============================================

GStepper2<STEPPER4WIRE> stepper(2048, 3, 4, 5, 6);

void work_motor()
{
  // stepper.setSpeedDeg(forward);
  stepper.setTargetDeg(forward);
  if (stepper.ready())
    stepper.disable();
}

void setup()
{
}

void loop()
{

  stepper.tick();
  work_motor();
}