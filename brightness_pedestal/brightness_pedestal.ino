/*
 * Control brightness of LEDs from a potentiometer, using PWM.
 *
 * Copyright (c) 2022 Aquitaine Simulation
 */

// Potentiometer pins
#define POT_INTEGLT A0
#define POT_FLOODMAIN A1
#define POT_FLOODPED A2

// Input pins
#define IN_BAT 2  // On BAT
#define IN_PWR 4  // Main power

// LED pins
#define LED_INTEGLT 3
#define LED_FLOODMAIN 5
#define LED_FLOODPED 6

// Output pin
#define OUT_PWR 7  // Copy of IN_PWR

// Brightness factor, increment above 0 to smooth logarithm curve
#define BRIGHTNESS_FACTOR 10


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
  pinMode(IN_BAT, INPUT);
  pinMode(IN_PWR, INPUT);
  pinMode(LED_INTEGLT, OUTPUT);
  pinMode(LED_FLOODMAIN, OUTPUT);
  pinMode(LED_FLOODPED, OUTPUT);
  pinMode(OUT_PWR, OUTPUT);

  log_multiplier = 255 / (1 - logn(1 + BRIGHTNESS_FACTOR, 1024 + BRIGHTNESS_FACTOR));
}

void loop() {
  int bat = digitalRead(IN_BAT);
  int pwr = digitalRead(IN_PWR);

  digitalWrite(OUT_PWR, pwr);

  if (bat == LOW || pwr == LOW) {
    set_led(POT_FLOODMAIN, LED_FLOODMAIN);
  } else {
    analogWrite(LED_FLOODMAIN, 0);
  }

  if (pwr == LOW) {
    set_led(POT_INTEGLT, LED_INTEGLT);
    set_led(POT_FLOODPED, LED_FLOODPED);
  } else {
    analogWrite(LED_INTEGLT, 0);
    analogWrite(LED_FLOODPED, 0);
  }

  delay(1);
}
