#include <GyverHX711.h>
#define PIN_DT 8
#define PIN_SCK 7
GyverHX711 sensor(PIN_DT, PIN_SCK, HX_GAIN128_A); // DT - 8; SCK - 7
// GyverHX711 sensor(10, 11, HX_GAIN128_A);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64




/*
scaleFactor - калибровочный коэффициент, зависит от усиления
1. приравнять scaleFactor равным 1
2. поставить известный вес на весы
3. полученное значение поделить на вес поставленный на весы (в идеале вязть среднее значение величины и поделить его на вес на весах)
*/

// float scaleFactor = 4036.4166;
float scaleFactor = 1;

float weight = 0;

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
  // чтение только по доступности! if available
  if (sensor.available())
  {

    weight = sensor.read() / scaleFactor;
    Serial.println(weight);
  }
}