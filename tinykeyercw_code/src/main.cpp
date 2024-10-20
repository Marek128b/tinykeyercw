#include <Arduino.h>

void setup()
{
  pinMode(PIN_PA2, OUTPUT);
}

void loop()
{
  digitalWrite(PIN_PA2, HIGH);
  delay(500);
  digitalWrite(PIN_PA2, LOW);
  delay(500);
}