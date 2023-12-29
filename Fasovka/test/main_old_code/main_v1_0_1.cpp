// работа кнопок заблокирована

#include <Arduino.h>

int16_t pause = 3000;     // задержка между шагами (мкс)
int8_t pins[] = {3, 4, 5, 6};  // драйвер (IN1 - A+, IN2 - A-, IN3 - B+, IN4 - B-)
#include <EncButton.h>

#define BTN_PIN 2           // кнопка
EncButton <EB_TICK, BTN_PIN> btn;


// выключаем ток на мотор
void disableMotor() {
  for (byte i = 0; i < 4; i++) digitalWrite(pins[i], 0);
}

// Состояние пинов на каждом шаге  {IN1, IN2, IN3, IN4}
int8_t t1[]={0,1,1,0};
int8_t t2[]={1,0,1,0};
int8_t t3[]={1,0,0,1};
int8_t t4[]={0,1,0,1};

void step1(){
  for(byte i = 0; i < 4; i++) {
  digitalWrite(pins[i],t1[i]);
  delayMicroseconds(pause);
}
}

void step2(){
  for(byte i = 0; i < 4; i++) {
  digitalWrite(pins[i],t2[i]);
  delayMicroseconds(pause);
}
}

void step3(){
  for(byte i = 0; i < 4; i++) {
  digitalWrite(pins[i],t3[i]);
  delayMicroseconds(pause);
}
}

void step4(){
  for(byte i = 0; i < 4; i++) {
  digitalWrite(pins[i],t4[i]);
  delayMicroseconds(pause);
}
}


void setup() {
  for (byte i = 0; i < 4; i++) pinMode(pins[i], OUTPUT); 
btn.setHoldTimeout(2000);

}

void loop() {
   btn.tick();
  if (btn.hasClicks(1)) pause = 1000; 
    if (btn.hasClicks(2)) pause = 2000; 
      if (btn.hasClicks(3)) pause = 3000; 

  step1();
  step2();
  step3();
  step4();
}