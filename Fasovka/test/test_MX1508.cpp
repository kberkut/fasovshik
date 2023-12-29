
#include <Arduino.h>

bool flagDirection = true;
bool flagOnce = false;
uint32_t timer = 0;
uint16_t pause = 1500;

void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{

  if ((millis() - timer >= pause))
  {
    timer = millis();
    flagDirection = !flagDirection;
    flagOnce = true;
  }

  if (flagOnce == true && flagDirection == true)
  {
    flagOnce = false;
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }

  if (flagOnce == true && flagDirection == false)
  {
    flagOnce = false;
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }
}