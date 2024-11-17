/*
 * Control brightness of LEDs from a potentiometer, using PWM.
 *
 * Copyright (c) 2022 Aquitaine Simulation
 */

// Potentiometer pin
#define POT_FCU A0
#define POT_LIGHTL A1
#define POT_LIGHTR A2

// Input pin
#define IN_PWR 2 // Main power

// LED pin
#define LED_FCU 3
#define LED_LIGHTL 5
#define LED_LIGHTR 6

// Brightness factor, increment above 0 to smooth logarithm curve
#define BRIGHTNESS_FACTOR 5


double log_multiplier;

double logn(unsigned int n, unsigned int base) {
  return log(n) / log(base);
}

void set_led(int pot_pin, int led_pin) {
  int pot = analogRead(pot_pin);
  double led_value = 1 - logn(1024 + BRIGHTNESS_FACTOR - pot, 1024 + BRIGHTNESS_FACTOR);
  int led_output = led_value * log_multiplier;
  analogWrite(led_pin, led_output);
}

void setup() {
  pinMode(IN_PWR, INPUT);
  pinMode(LED_FCU, OUTPUT);
  pinMode(LED_LIGHTL, OUTPUT);
  pinMode(LED_LIGHTR, OUTPUT);

  log_multiplier = 255 / (1 - logn(1 + BRIGHTNESS_FACTOR, 1024 + BRIGHTNESS_FACTOR));
}

void loop() {
  int pwr = digitalRead(IN_PWR);

  if (pwr == LOW) {
    set_led(POT_FCU, LED_FCU);
    set_led(POT_LIGHTL, LED_LIGHTL);
    set_led(POT_LIGHTR, LED_LIGHTR);
  } else {
    analogWrite(LED_FCU, 0);
    analogWrite(LED_LIGHTL, 0);
    analogWrite(LED_LIGHTR, 0);
  }

  delay(1);
}
