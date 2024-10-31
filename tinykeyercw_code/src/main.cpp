#include <Arduino.h>

/* defining the pin assignments for different components */
int dit_pin = PIN_PA6;
int dah_pin = PIN_PA5;
int led_pin = PIN_PA2;
int buzzer_pin = PIN_PA1;

/* These lines of code are initializing variables used in the Morse code generator program*/
byte wpm = 15;
int interval_time = 1500 / wpm; // WPM to interval
int freq_buzz = 1000;
int interval_time_buzz = 1000;
unsigned long long last_millis = 0;
boolean buzzerState = false;
unsigned long long last_micros_buzz = 0;
boolean buzzerStatePWM = false;
byte stateloop = 0;

void setup()
{
  /* calculating the interval time for the buzzer to create a specific frequency
  sound. It is converting the frequency value `freq_buzz` into the corresponding time interval in
  microseconds. The formula used is `(1 / freq_buzz) * 1000000`, where `1 / freq_buzz` calculates
  the period of one cycle of the frequency, and then multiplying by `1000000` converts it to
  microseconds. This interval time is then used to control the buzzer's PWM (Pulse Width Modulation)
  to generate the desired frequency sound. */
  interval_time_buzz = (float)(1 / (float)freq_buzz) * 1000000;
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(dit_pin, INPUT);
  pinMode(dah_pin, INPUT);
}

void loop()
{
  // Morse Code Keyer
  // This code reads inputs from two switches ("dit" and "dah") representing Morse code elements.
  // When a switch is pressed, it activates a buzzer for a specified duration to produce Morse code sounds.
  switch (stateloop)
  {
  case 0: // checking states
    // Check if either the "dit" or "dah" switch is pressed.
    if (!digitalRead(dit_pin)) // If "dit" (short beep) switch is pressed
    {
      stateloop = 1; // Move to "dit" state
    }
    if (!digitalRead(dah_pin)) // If "dah" (long beep) switch is pressed
    {
      stateloop = 2; // Move to "dah" state
    }
    if (!digitalRead(dah_pin) && !digitalRead(dit_pin))
    {
      stateloop = 4;
    }
    last_millis = millis(); // Record the current time
    break;
  case 1:                                        // Dit state (produces a short beep)
    buzzerState = true;                          // Turn on the buzzer
    if (millis() - last_millis >= interval_time) // If a third of interval time has passed
    {
      buzzerState = false;    // Turn off the buzzer
      stateloop = 3;          // Move to the delay state (inter-element gap)
      last_millis = millis(); // Record the current time
    }
    break;
  case 2:                                            // Dah state (produces a long beep)
    buzzerState = true;                              // Turn on the buzzer
    if (millis() - last_millis >= interval_time * 3) // If the full interval time has passed
    {
      buzzerState = false;    // Turn off the buzzer
      stateloop = 3;          // Move to the delay state (inter-element gap)
      last_millis = millis(); // Record the current time
    }
    break;
  case 3:                                        // Delay state (pause between Morse elements)
    buzzerState = false;                         // Ensure the buzzer is off
    if (millis() - last_millis >= interval_time) // If a third of interval time has passed
    {
      stateloop = 0; // Return to the waiting state
    }
    break;
  case 4:               // dit dah dit dah dit dah
    buzzerState = true; // Turn on the buzzer
    if (millis() - last_millis >= interval_time)
    {
      buzzerState = false; // Turn off the buzzer
      stateloop = 5;
      last_millis = millis();
    }
    break;
  case 5:                                        // dit dah dit dah dit dah pause
    buzzerState = false;                         // Ensure the buzzer is off
    if (millis() - last_millis >= interval_time) // If a third of interval time has passed
    {
      stateloop = 6; // Return to the waiting state
    }
    break;
  case 6:               // dit dah dit dah dit dah
    buzzerState = true; // Turn on the buzzer
    if (millis() - last_millis >= interval_time * 3)
    {
      buzzerState = false; // Turn off the buzzer
      stateloop = 3;
      last_millis = millis();
    }
    break;
  default:         // Default state (failsafe)
    stateloop = 0; // Reset to the waiting state
    break;
  }

  // If buzzer is active, turn on LED and generate a PWM signal for buzzer
  if (buzzerState)
  {
    digitalWrite(led_pin, HIGH); // LED on to indicate buzzer activity

    // Generate PWM for buzzer by toggling `buzzer_pin` at intervals
    if (micros() - last_micros_buzz >= interval_time_buzz) // Check if PWM interval elapsed
    {
      buzzerStatePWM = !buzzerStatePWM;         // Toggle PWM state
      digitalWrite(buzzer_pin, buzzerStatePWM); // Update buzzer output
      last_micros_buzz = micros();              // Reset PWM timer
    }
  }
  else
  {
    digitalWrite(buzzer_pin, LOW); // Turn buzzer off
    digitalWrite(led_pin, LOW);    // Turn LED off
  }
}