#include <Arduino.h>

// ===================== PIN SETUP =====================
const int potPin = A0;   // pin analog potensiometer
const int ledPin = 9;    // pin PWM untuk LED

// ===================== VARIABEL =====================
int nilaiADC = 0;  // nilai awal ADC
int pwm = 0;       // nilai PWM

void setup() {

  // ===================== OUTPUT SETUP =====================
  pinMode(ledPin, OUTPUT);

  // ===================== SERIAL MONITOR =====================
  Serial.begin(9600);
}

void loop() {

  // ===================== PEMBACAAN SENSOR =====================
  nilaiADC = analogRead(potPin);

  // ===================== PEMROSESAN DATA =====================
  pwm = map(nilaiADC,
            0,    // minimum ADC
            1023, // maksimum ADC
            0,    // PWM minimum
            255); // PWM maksimum

  // ===================== OUTPUT PWM =====================
  analogWrite(ledPin, pwm);

  // ===================== MONITORING DATA =====================
  Serial.print("ADC: ");
  Serial.print(nilaiADC);

  Serial.print(" | PWM: ");
  Serial.println(pwm);

  // ===================== STABILISASI =====================
  delay(50);
}