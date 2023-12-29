// https://habr.com/ru/articles/453276/

#include <Arduino.h>

#define LEDPIN 13

#include "GyverStepper2.h"
GStepper2<STEPPER2WIRE> stepper(2048, 3, 4);
bool bool_tmr1 = 1;
bool bool_tmr2 = 0;
uint32_t tmr1 = millis();
#define STEPPER_PIN_START 11

#include <EncButton.h>
#define BTN_PIN_START 12
EncButton<EB_TICK, BTN_PIN_START> btn1;
bool flagStart = false;
bool flagStaus = false;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
uint32_t timerLCDredraw = 0;

#include <TimerMs.h>
TimerMs tmr(7, 1, 0);

void LCD()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("11111111");
}

void setup()
{
  // инициализация Timer1
  cli();      // отключить глобальные прерывания
  TCCR1A = 0; // установить TCCR1A регистр в 0
  TCCR1B = 0;

  // включить прерывание Timer1 overflow:
  TIMSK1 = (1 << TOIE1);
  // Установить CS10 бит так, чтобы таймер работал при тактовой частоте:

  TCCR1B |= (0 << CS10); // 1
  TCCR1B |= (0 << CS11); // 0
  TCCR1B |= (1 << CS12); // 0

  sei(); // включить глобальные прерывания

  pinMode(LEDPIN, OUTPUT);

  Serial.begin(9600);
  pinMode(STEPPER_PIN_START, OUTPUT);
  digitalWrite(STEPPER_PIN_START, LOW);
  lcd.init();
  lcd.backlight();
  stepper.reverse(1);

  tmr.setMicros(true);
  tmr.setPeriodMode();
}

ISR(TIMER1_OVF_vect)
{
  stepper.tick();
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}

void loop()
{
  btn1.tick();

  if (btn1.press())
  {
    flagStart = !flagStart;
    flagStaus = !flagStaus;

  }

  if (flagStart == true && flagStaus == true)
  {
    stepper.setSpeedDeg(60);
    digitalWrite(STEPPER_PIN_START, LOW);
    flagStaus = !flagStaus;
  }
  if (flagStart == false && flagStaus == true)
  {
    digitalWrite(STEPPER_PIN_START, HIGH);
    flagStaus = !flagStaus;
  }

  if (millis() - timerLCDredraw >= 500)
  {
    timerLCDredraw = millis();
    LCD();
  }
}
