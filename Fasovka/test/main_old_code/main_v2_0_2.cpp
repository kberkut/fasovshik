  // Не доделано. Блокируется вызов кнопок.
#include <Arduino.h>
#include <EncButton.h>
#include <TimerMs.h>

#define BTN_PIN 2           // кнопка
#define FEED_SPEED 3000     // задержка между шагами мотора (мкс)
#define STEPS_FRW 25        // шаги вперёд
#define STEPS_BKW 6        // шаги назад
const byte drvPins[] = {3, 4, 5, 6};  // драйвер (фазаА1, фазаА2, фазаВ1, фазаВ2)
int timer_feed = micros();
byte flagStart = false;

EncButton <EB_TICK, BTN_PIN> btn;
int feedAmount = 1000;
TimerMs tmr(10, 1, 0);

const byte steps[] = {0b0110, 0b1010, 0b1001, 0b0101};
void runMotor(int8_t dir) {
  if(tmr.tick()) {
  static byte step = 0;
  for (byte i = 0; i < 4; i++) digitalWrite(drvPins[i], bitRead(steps[step & 0b11], i));
    // delayMicroseconds(FEED_SPEED);
     step += dir;
  }
}

// выключаем ток на мотор
void disableMotor() {
  for (byte i = 0; i < 4; i++) digitalWrite(drvPins[i], 0);
}

void oneRev() {
  for (int i = 0; i < STEPS_BKW; i++) runMotor(1);
  for (int i = 0; i < STEPS_FRW; i++) runMotor(-1);
}

void feed() {
  for (int i = 0; i < feedAmount; i++)  oneRev(); 
  disableMotor();
}

void setup() {
  for (byte i = 0; i < 4; i++) pinMode(drvPins[i], OUTPUT);   // пины выходы
}

void loop() {
   btn.tick();
  if (flagStart && btn.click()) {flagStart = !flagStart; disableMotor();}
  if (!flagStart && btn.click()) flagStart = !flagStart;
  if(flagStart) feed(); 
}







