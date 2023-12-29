#include <Arduino.h>
#include <EncButton.h>
#define BTN_PIN 2           // кнопка
EncButton <EB_TICK, BTN_PIN> btn;

#define pause 1000     // задержка между шагами (мкс)
int8_t pins[] = {3, 4, 5, 6};  // драйвер (IN1 - A+, IN2 - A-, IN3 - B+, IN4 - B-)
byte flagStep1 = true;
byte flagStep2 = false;
byte flagStep3 = false;
byte flagStep4 = false;
byte flagStart = false;

// выключаем ток на мотор
void disableMotor() {
  for (byte i = 0; i < 4; i++) digitalWrite(pins[i], 0);
}

// Состояние пинов на каждом шаге  {IN1, IN2, IN3, IN4}
byte stepIN[4][4]={{0,1,1,0},{1,0,1,0},{1,0,0,1},{0,1,0,1}};




void step(){
    static byte stepIN_1[4];
  for(byte i = 0; i < 4; i++) {
    memcpy(stepIN_1, stepIN[i], 4);
    for(byte i = 0; i < 4; i++) {

        digitalWrite(pins[i],stepIN_1[i]);
    // delayMicroseconds(3000);

    }
    delayMicroseconds(1000);

  }
}



void setup() {
  for (byte i = 0; i < 4; i++) pinMode(pins[i], OUTPUT); 
}

// void timerStep1() {
//   static uint32_t timer = 0;

// if (flagStep1 && (micros() - timer >= pause)) {
// timer = micros(); flagStep1 = false; flagStep2 = true;
// }
// if (flagStep2 && (micros() - timer >= pause)) {
// timer = micros(); flagStep2 = false; flagStep3 = true;
// }
// if (flagStep3 && (micros() - timer >= pause)) {
// timer = micros(); flagStep3 = false; flagStep4 = true;
// }
// if (flagStep4 && (micros() - timer >= pause)) {
// timer = micros(); flagStep4 = false; flagStep1 = true;
// }
// }

void loop() {
//   timerStep1();
     btn.tick();
  if (flagStart && btn.click()) {flagStart = false; disableMotor();}
  if (!flagStart && btn.click()) flagStart = true;
  if(flagStart) {
    step();
    //   if(flagStep1) step1();
    //   if(flagStep2) step3();
    //   if(flagStep3) step3();
    //   if(flagStep4) step4();
  }
}

