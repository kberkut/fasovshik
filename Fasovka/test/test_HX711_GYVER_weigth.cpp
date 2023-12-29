// Весы на основе библиотеки. Отработка алгоритма для фасовщика.
#include <GyverHX711.h>
GyverHX711 sensor(8, 7, HX_GAIN128_A);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64

#include <EncButton.h>
#define BTN_PIN_START A0
EncButton<EB_TICK, BTN_PIN_START> btn1;

/*
scaleFactor - калибровочный коэффициент, зависит от усиления
1. приравнять scaleFactor равным 1
2. поставить известный вес на весы
3. полученное значение поделить на вес поставленный на весы (в идеале вязть среднее значение величины и поделить его на вес на весах)
*/

float scaleFactor = 4036.4166;
float weight = 0;
uint32_t timer = 0;

void setup()
{
  Serial.begin(57600);

  // если тарирование при первом запуске -
  // нужно выждать готовность датчика
  delay(500);
  sensor.tare(); // калибровка нуля

  // sensor.sleepMode(true);		// выключить датчик
  // sensor.sleepMode(false);	// включить датчик
}

void loop()
{
  btn1.tick();
  // чтение только по доступности! if available
  if (sensor.available() && (millis() - timer >= 100))
  {
    timer = millis();
    weight = sensor.read() / scaleFactor;
    Serial.println(weight);
  }
  if (btn1.click())
    sensor.tare();

  if (weight >= 500)
  {
    Serial.println("Вес достигнут");
    sensor.sleepMode(true);
  }
}