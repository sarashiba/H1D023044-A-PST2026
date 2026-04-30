#include <Servo.h>

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // pin analog untuk potensiometer
const int servoPin = 9;       // pin digital PWM untuk servo

// ===================== VARIABEL =====================
int pos = 0; // sudut servo
int val = 0; // nilai ADC dari potensiometer

void setup() {

  // Hubungkan servo ke pin
  myservo.attach(servoPin);

  // Serial monitor
  Serial.begin(9600);

}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  val = analogRead(potensioPin); // baca nilai potensiometer

  // ===================== KONVERSI DATA =====================
  pos = map(val, 
            0,    // nilai minimum ADC
            1023, // nilai maksimum ADC
            0,    // sudut minimum servo
            180); // sudut maksimum servo

  // ===================== OUTPUT SERVO =====================
  myservo.write(pos);

  // ===================== MONITORING DATA =====================
  Serial.print("ADC Potensio: ");
  Serial.print(val);

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  // ===================== STABILISASI =====================
  delay(15); // delay kecil agar servo stabil
}