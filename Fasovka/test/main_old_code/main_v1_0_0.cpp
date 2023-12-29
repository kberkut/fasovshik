#include <Arduino.h>
#include <EncButton.h>

#define BTN_PIN 2           // кнопка
EncButton <EB_TICK, BTN_PIN> btn;
byte flagStart = false;

#define pause 3000     // задержка между шагами (мкс)
int8_t pins[] = {3, 4, 5, 6};  // драйвер (IN1 - A+, IN2 - A-, IN3 - B+, IN4 - B-)


// Состояние пинов на каждом шаге  {IN1, IN2, IN3, IN4}
int8_t t1[]={0,1,1,0};
int8_t t2[]={1,0,1,0};
int8_t t3[]={1,0,0,1};
int8_t t4[]={0,1,0,1};

// выключаем ток на мотор
void disableMotor() {
  for (byte i = 0; i < 4; i++) digitalWrite(pins[i], 0);
}

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
}

void loop() {

  btn.tick();
  if (!flagStart && btn.click()) flagStart = true;
  if (flagStart && btn.click()) flagStart = false;
  if(!flagStart) disableMotor();
  if(flagStart) {
    step1();
    step2();
    step3();
    step4();
  }
}