// https://circuitdigest.com/microcontroller-projects/portable-weighing-machine-using-arduino-and-hx711-weight-sensor-load-cell

#include <HX711_ADC.h>
// #include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 16);

const int HX711_dout = 10; // mcu > HX711 dout pin
const int HX711_sck = 11;  // mcu > HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);
int tpin = 2; // tare pin
// const int calVal_eepromAdress = 0;
long t;
const int Up_buttonPin = A0; // the pin that the pushbutton is attached to
const int Down_buttonPin = A1;
float buttonPushCounter = 0;    // counter for the number of button presses
float up_buttonState = 0;       // current state of the up button
float up_lastButtonState = 0;   // previous state of the up button
float down_buttonState = 0;     // current state of the up button
float down_lastButtonState = 0; // previous state of the up button
bool bPress = false;

const int LED_RED = A2;
const int LED_BLUE = A3;

void checkUp()
{
  up_buttonState = digitalRead(Up_buttonPin);
  // compare the buttonState to its previous state
  if (up_buttonState != up_lastButtonState)
  {
    // if the state has changed, increment the counter
    if (up_buttonState == LOW)
    {
      bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter = buttonPushCounter + 10;
    }
  }
  // save the current state as the last state, for next time through the loop
  up_lastButtonState = up_buttonState;
}
void checkDown()
{
  down_buttonState = digitalRead(Down_buttonPin);
  // compare the buttonState to its previous state
  if (down_buttonState != down_lastButtonState)
  {
    // if the state has changed, increment the counter
    if (down_buttonState == LOW)
    {
      bPress = true;
      buttonPushCounter = buttonPushCounter - 10;
    }
  }
  // save the current state as the last state, for next time through the loop
  down_lastButtonState = down_buttonState;
}

void setup()
{
  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
  pinMode(tpin, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(Up_buttonPin, INPUT_PULLUP);
  pinMode(Down_buttonPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ARDUINO BALANCE");
  lcd.setCursor(0, 1);
  lcd.print("let's measure");
  delay(2000);
  lcd.clear();
  LoadCell.begin();
  float calibrationValue;     // calibration value (see example file "Calibration.ino")
  calibrationValue = 4035.33; // 260.14 gramm      uncomment this if you want to set the calibration value in the sketch
                              // #if defined(ESP8266) || defined(ESP32)
  // EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
  // #endif
  // EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
  long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true;        // set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag())
  {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1)
      ;
  }
  else
  {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}
void loop()
{
  static boolean newDataReady = 0;
  const int serialPrintInterval = 20; // increase value to slow down serial print activity
  // check for new data/start next conversion:
  if (LoadCell.update())
    newDataReady = true;
  // get smoothed value from the dataset:
  if (newDataReady)
  {
    if (millis() > t + serialPrintInterval)
    {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
      lcd.setCursor(0, 0);
      lcd.print("set wei:");
      lcd.setCursor(9, 0);
      lcd.print(buttonPushCounter);
      lcd.setCursor(14, 0);
      lcd.print("GM");
      lcd.setCursor(0, 1);
      lcd.print("weight :");
      lcd.setCursor(9, 1);
      lcd.print(i);
      lcd.setCursor(14, 1);
      lcd.print("GM");
    }
  }
  checkUp();
  checkDown();
  if (digitalRead(tpin) == LOW)
  {
    LoadCell.tareNoDelay();
  }
  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true)
  {
    lcd.clear();
    lcd.print("Tare complete");
    delay(1000);
    lcd.clear();
  }
  float i = LoadCell.getData();
  float k = buttonPushCounter - i;
  if (k < 50 && k > 1)
  {
    digitalWrite(LED_RED, HIGH);
    delay(50);
    digitalWrite(LED_RED, LOW);
    delay(50);
  }
  if (k >= 50)
  {
    digitalWrite(LED_RED, HIGH);
    delay(200);
    digitalWrite(LED_RED, LOW);
    delay(200);
  }
  if (i >= buttonPushCounter)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, HIGH);
  }
  else
  {
    digitalWrite(LED_BLUE, LOW);
  }
}
