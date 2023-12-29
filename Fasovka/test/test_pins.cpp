#include <Arduino.h>

#include <EncButton.h>
#define BTN_PIN_START 12
EncButton<EB_TICK, BTN_PIN_START> btn1;
uint16_t test = 0;

void setup()
{
  Serial.begin(57600);
  pinMode(13, OUTPUT);
}
void loop()
{
  btn1.tick();

  if (btn1.press())
  {
    Serial.println("Нажание кнопки");
    Serial.println(test);
    digitalWrite(13, HIGH);
    test++;
    delay(500);
  }
  else
    digitalWrite(13, LOW);
}
