# Jawaban Pertanyaan Praktikum Modul 4 Percobaan 1

## 1. Fungsi Perintah `analogRead()`
Pada rangkaian praktikum ini, fungsi `analogRead()` digunakan untuk membaca sinyal tegangan analog dari potensiometer yang terhubung pada pin analog Arduino (pin A0). Mikrokontroler memanfaatkan fitur *Analog to Digital Converter* (ADC) beresolusi 10-bit untuk menerjemahkan variasi tegangan analog (umumnya 0–5 Volt) tersebut menjadi nilai data digital diskrit dalam rentang **0 hingga 1023** agar dapat diproses lebih lanjut oleh sistem.

## 2. Alasan Diperlukannya Fungsi `map()`
Fungsi `map()` diperlukan untuk menyamakan perbedaan skala (rentang) antara data input ADC dan data output aktuator motor servo. Nilai hasil pembacaan potensiometer berada pada rentang **0 hingga 1023**, sedangkan motor servo standar hanya dapat merespons perintah pergerakan sudut dalam rentang **0 hingga 180 derajat**. Apabila fungsi `map()` tidak digunakan, nilai input yang dikirim ke servo dapat melebihi batas 180 derajat dan menyebabkan error pergerakan atau kerusakan mekanis pada motor. Fungsi ini melakukan konversi secara proporsional dan linear dari rentang 0–1023 menjadi 0–180 (atau batas tertentu).

## 3. Modifikasi Program (Rentang 30° hingga 150°) dan Penjelasan Program
Berikut adalah modifikasi program agar servo hanya bergerak pada rentang 30° hingga 150°:
![Hasil modifikasi](modifikasi_adc.gif)

```cpp
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

//  PIN SETUP
const int potensioPin = A0;   // pin analog input untuk potensiometer
const int servoPin = 9;       // pin digital untuk servo (PWM)

// VARIABEL
int pos = 0; // variabel untuk menyimpan sudut servo
int val = 0; // variabel untuk menyimpan nilai ADC potensiometer

void setup() {

  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(servoPin);

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(9600);

}

void loop() {

  // PEMBACAAN ADC 
  // Baca nilai dari potensiometer (rentang 0–1023)
  val = analogRead(potensioPin);

  // KONVERSI DATA 
  // Ubah nilai ADC menjadi sudut servo (dimodifikasi ke 30-150 derajat)
  pos = map(val,
            0,    // nilai minimum ADC
            1023, // nilai maksimum ADC
            30,   // sudut minimum servo
            150); // sudut maksimum servo

  //  OUTPUT SERVO 
  // Gerakkan servo sesuai hasil mapping
  myservo.write(pos);

  // MONITORING DATA 
  // Tampilkan data ADC dan sudut servo ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val);

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  //  STABILISASI 
  // Delay untuk memberi waktu servo bergerak stabil
  delay(15);
}
```

### Penjelasan Program
Program ini dirancang untuk mengontrol pergerakan motor servo berdasarkan input putaran dari potensiometer dengan memanfaatkan fitur *Analog to Digital Converter* (ADC) pada Arduino. 

**Alur kerja dari program di atas adalah sebagai berikut:**
1. **Input (Pembacaan ADC):** Mikrokontroler membaca variasi tegangan dari potensiometer secara terus-menerus melalui pin A0 menggunakan instruksi `analogRead()`. Sinyal fisik ini dikonversi menjadi data digital 10-bit yang menghasilkan nilai dari **0 hingga 1023**.
2. **Proses (Konversi / Mapping):** Karena nilai ADC 0–1023 tidak dapat digunakan secara langsung untuk menentukan sudut servo, program menggunakan fungsi `map()`. Pada kode yang telah dimodifikasi ini, nilai batas bawah ADC (0) dikonversi secara proporsional menjadi sudut **30°**, dan nilai batas atas ADC (1023) dikonversi menjadi sudut **150°**.
3. **Output (Aktuasi Servo):** Hasil perhitungan sudut matematis tersebut disimpan pada variabel `pos` dan diteruskan ke motor servo melalui perintah `myservo.write(pos)`. Hal ini membuat pergerakan servo selalu linear dan sejajar dengan persentase putaran potensiometer.
4. **Monitoring Data:** Secara *real-time*, nilai ADC sumber dan derajat sudut hasil konversi dicetak ke **Serial Monitor** (dengan kecepatan 9600 bps). Fitur ini digunakan untuk memastikan konversi dan pembacaan sensor beroperasi secara akurat.

Pembatasan rentang pada fungsi `map()` menjadi 30–150 bertujuan agar motor servo memiliki batasan keamanan secara *software*. Melalui metode ini, perangkat terhindar dari paksaan putaran hingga titik mekanis maksimalnya, meskipun potensiometer perangkat keras diputar secara penuh.