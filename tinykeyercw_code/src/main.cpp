#include <Arduino.h>

int dit_pin = PIN_PA6;
int dah_pin = PIN_PA5;
int led_pin = PIN_PA2;
int buzzer_pin = PIN_PA1;

int interval_time = 80;
int interval_multiplier = 2;
unsigned long long last_millis = 0;
unsigned long long last_millis2 = 0;

int freq_buzz = 600;
int interval_time_buzz = 1000;
unsigned long long last_micros_buzz = 0;
boolean buzzerStatePWM = false;

const byte bufferSize = 8;
boolean inputBuffer[bufferSize];
boolean bufferState = false;

void shiftBuffer();

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
  if (millis() - last_millis >= interval_time * interval_multiplier)
  {
    if (!digitalRead(dit_pin)) // dit code ..10..
    {
      inputBuffer[bufferSize-1] = 1;
      shiftBuffer();
      inputBuffer[bufferSize-1] = 0;
      shiftBuffer();
      interval_multiplier = 2;
    }
   if (!digitalRead(dah_pin)) // dah code ..1110..
    {
      inputBuffer[bufferSize-1] = 1;
      shiftBuffer();
      inputBuffer[bufferSize-1] = 1;
      shiftBuffer();
      inputBuffer[bufferSize-1] = 1;
      shiftBuffer();
      inputBuffer[bufferSize-1] = 0;
      shiftBuffer();
      interval_multiplier = 4;
    }
    last_millis = millis();
  }

  if (millis() - last_millis2 >= interval_time)
  {
    bufferState = inputBuffer[0];
    shiftBuffer();
    last_millis2 = millis();
  }

  digitalWrite(led_pin, bufferState);
  if (bufferState)
  {
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
  }
}

void shiftBuffer()
{
  for (int i = 0; i < bufferSize; i++)
  {
    if (i == bufferSize-1)
    {
      inputBuffer[i] = 0;
    }
    else
    {
      inputBuffer[i] = inputBuffer[i + 1];
    }
  }
}