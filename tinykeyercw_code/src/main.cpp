#include <Arduino.h>

int dit_pin = PIN_PA6;
int dah_pin = PIN_PA5;
int led_pin = PIN_PA2;
int buzzer_pin = PIN_PA1;

int interval_time = 500;

unsigned long long last_millis = 0;
boolean buzzerState = false;

void setup()
{
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
      digitalWrite(led_pin, buzzerState);
      last_millis = millis();
    }
  }

  if (!digitalRead(dah_pin))
  {
    if (millis() - last_millis >= interval_time)
    {
      buzzerState = !buzzerState;
      digitalWrite(led_pin, buzzerState);
      last_millis = millis();
    }
  }

/*if (buzzerState)
  {
    tone(buzzer_pin, 1000);
  }
  else
  {
    noTone(buzzer_pin);
  }
*/
  
}