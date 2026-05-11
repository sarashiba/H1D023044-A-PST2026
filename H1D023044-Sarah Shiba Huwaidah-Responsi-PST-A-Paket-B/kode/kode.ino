#include <Servo.h>

Servo aktuatorSayap;

const int pinTombol = 3;
const int pinServo = 9;
const int pinLED = 8;

unsigned long waktuSebelumnya = 0;
const long jedaKirim = 500;

void setup() {
  Serial.begin(9600);

  pinMode(pinTombol, INPUT_PULLUP);
  pinMode(pinLED, OUTPUT);

  aktuatorSayap.attach(pinServo);
  
  // Posisi awal sayap tertutup rapat
  aktuatorSayap.write(0); 
}

void loop() {
  int statusTombol = digitalRead(pinTombol);
  unsigned long waktuSekarang = millis();

  // Logika saat tombol ditarik
  if (statusTombol == LOW) {
    aktuatorSayap.write(90);
    digitalWrite(pinLED, HIGH);

    // cek waktu kirim log serial
    if (waktuSekarang - waktuSebelumnya >= jedaKirim) {
      Serial.println("DRS: ACTIVE");
      waktuSebelumnya = waktuSekarang;
    }
  }
  
  // tombol dilepas atau diam
  else {
    aktuatorSayap.write(0);
    digitalWrite(pinLED, LOW);

    // mengirim log serial
    if (waktuSekarang - waktuSebelumnya >= jedaKirim) {
      Serial.println("DRS: CLOSED");
      waktuSebelumnya = waktuSekarang;
    }
  }
}