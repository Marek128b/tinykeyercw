#include <Arduino.h>

int dit_pin = PIN_PA6;
int dah_pin = PIN_PA5;
int led_pin = PIN_PA2;
int buzzer_pin = PIN_PA1;

int interval_time = 500;
int freq_buzz = 1000;
int interval_time_buzz = 1000;
unsigned long long last_millis = 0;
boolean buzzerState = false;
unsigned long long last_micros_buzz = 0;
boolean buzzerStatePWM = false;

void setup()
{
  interval_time_buzz = (float)(1 / (float)freq_buzz) * 1000000;
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(dit_pin, INPUT);
  pinMode(dah_pin, INPUT);
}

void loop()
{
  if (!digitalRead(dit_pin))
  {
    if (millis() - last_millis >= interval_time / 3)
    {
      buzzerState = !buzzerState;
      last_millis = millis();
    }
  }

  if (!digitalRead(dah_pin))
  {
    if (millis() - last_millis >= interval_time)
    {
      buzzerState = !buzzerState;
      last_millis = millis();
    }
  }

  if (buzzerState)
  {

    digitalWrite(led_pin, HIGH);
    if (micros() - last_micros_buzz >= interval_time_buzz)
    {
      buzzerStatePWM = !buzzerStatePWM;
      digitalWrite(buzzer_pin, buzzerStatePWM);
      last_micros_buzz = micros();
    }
  }
  else
  {
    digitalWrite(buzzer_pin, LOW);
    digitalWrite(led_pin, LOW);
  }

  if (digitalRead(dit_pin) && digitalRead(dah_pin))
  {
    buzzerState = false;
  }
}