# Jawaban Pertanyaan Praktikum Modul 4 Percobaan 2

## 1. Penjelasan Mengenai Kecerahan LED Menggunakan Fungsi `analogWrite()`
Fungsi `analogWrite()` pada Arduino digunakan untuk menghasilkan sinyal PWM (*Pulse Width Modulation*). Sinyal PWM bekerja dengan cara menghidupkan (kondisi HIGH/5V) dan mematikan (kondisi LOW/0V) arus pada pin digital dengan frekuensi yang sangat cepat. Perbandingan antara lamanya waktu pin berada dalam kondisi HIGH terhadap total waktu satu siklus disebut sebagai *duty cycle*. 

Ketika nilai PWM yang diberikan tinggi (mendekati 255), rentang waktu sinyal berada di posisi HIGH akan lebih lama dibandingkan posisi LOW, sehingga rata-rata tegangan listrik yang diterima oleh LED menjadi lebih besar dan LED menyala lebih terang. Sebaliknya, nilai PWM yang rendah menghasilkan rata-rata tegangan yang lebih kecil sehingga LED meredup. Kedipan yang sangat cepat ini tidak dapat ditangkap oleh mata manusia, sehingga yang terlihat hanyalah ilusi perubahan intensitas cahaya atau kecerahan.

## 2. Hubungan Antara Nilai ADC (0–1023) dan Nilai PWM (0–255)
Hubungan antara keduanya didasari oleh perbedaan resolusi perangkat keras pada mikrokontroler Arduino Uno.
- **Nilai ADC (0–1023)** berasal dari fitur pembacaan analog (*Analog to Digital Converter*) yang memiliki resolusi **10-bit** ($2^{10} = 1024$ nilai).
- **Nilai PWM (0–255)** merupakan batasan dari keluaran fungsi *analogWrite* yang menggunakan resolusi **8-bit** ($2^8 = 256$ nilai).

Karena adanya perbedaan skala resolusi ini, rentang input (ADC) tidak bisa langsung dimasukkan sebagai perintah output (PWM). Hubungan keduanya diselaraskan secara linear menggunakan fungsi `map()`, sehingga putaran potensiometer secara penuh dari kiri ke kanan (0-1023) dapat merepresentasikan perubahan *duty cycle* secara penuh dari 0% ke 100% (0-255). Secara matematis, nilai PWM didapat dengan membagi nilai ADC dengan 4.

## 3. Modifikasi Program (Rentang Kecerahan 50–200) dan Penjelasan Program
Berikut adalah modifikasi program (`modul4_percobaan2.ino`) agar LED hanya beroperasi pada rentang kecerahan sedang (PWM 50 hingga 200):
![Hasil modifikasi](modifikasi_pwm.gif)
```cpp
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
  // Modifikasi: Mengubah batas minimum PWM menjadi 50 dan maksimum 200
  pwm = map(nilaiADC,
            0,    // minimum ADC
            1023, // maksimum ADC
            50,   // PWM minimum (batas bawah kecerahan)
            200); // PWM maksimum (batas atas kecerahan)

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
```

### Penjelasan Program
Program ini berfungsi untuk mengontrol tingkat kecerahan komponen LED berdasarkan posisi putaran potensiometer menggunakan metode PWM (*Pulse Width Modulation*).

**Alur kerja program:**
1. **Inisialisasi:** Pada fungsi `setup()`, pin 9 diatur sebagai jalur output untuk LED, dan komunikasi serial diaktifkan dengan *baud rate* 9600 untuk kebutuhan monitoring.
2. **Pembacaan Sensor (Input):** Mikrokontroler membaca sinyal analog dari potensiometer melalui pin A0. Data ini merupakan tegangan listrik yang telah dikonversi oleh ADC menjadi nilai digital dari **0 hingga 1023**, yang kemudian disimpan dalam variabel `nilaiADC`.
3. **Pemrosesan Data (Mapping):** Karena program disyaratkan untuk membatasi kecerahan LED agar tidak mati total dan tidak terlalu terang maksimal, rentang output dimodifikasi. Fungsi `map()` menerjemahkan nilai batas minimal ADC (0) menjadi nilai PWM **50** (kecerahan redup batas bawah), dan nilai maksimal ADC (1023) menjadi nilai PWM **200** (kecerahan sedang batas atas).
4. **Eksekusi Output:** Nilai hasil pemetaan tersebut disimpan pada variabel `pwm` dan kemudian diinstruksikan ke pin LED menggunakan fungsi `analogWrite()`. Rata-rata tegangan yang dikeluarkan pin akan otomatis menyesuaikan nilai PWM yang berkisar antara 50 hingga 200.
5. **Monitoring:** Untuk memudahkan analisis dan *troubleshooting*, program secara kontinu mencetak perbandingan antara nilai sensor `nilaiADC` dan nilai keluaran `pwm` pada **Serial Monitor**, diakhiri dengan jeda `delay(50)` untuk memberikan kestabilan pembacaan.