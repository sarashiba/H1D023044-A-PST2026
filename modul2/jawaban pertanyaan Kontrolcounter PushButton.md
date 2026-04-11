# Jawaban Pertanyaan Praktikum Kontrol Counter PushButton 7Segment

### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
<img width="884" height="690" alt="image" src="https://github.com/user-attachments/assets/2cf5d472-f7cf-4c96-b958-bfd292302f90" />

---

### 2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno? Apa keuntungannya dibandingkan rangkaian biasa?

Penggunaan mode INPUT_PULLUP berfungsi untuk mengaktifkan resistor *pull-up* internal yang sudah tersedia di dalam mikrokontroler Arduino. 
Keuntungan utamanya adalah kita tidak perlu lagi merangkai resistor eksternal secara manual di breadboard sehingga rangkaian menjadi lebih ringkas dan hemat kabel. 
Selain itu mode ini memastikan pin selalu mendapat suplai tegangan default atau HIGH saat tombol sedang tidak ditekan. Hal ini sangat penting untuk mencegah terjadinya 
status *floating* atau sinyal mengambang yang bisa menyebabkan Arduino salah membaca input akibat gangguan elektromagnetik di sekitar rangkaian.

---

### 3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?

Dari sisi perangkat keras atau *hardware*, penyebabnya bisa bermacam-macam
* Kabel jumper yang mengarah ke pin segmen tersebut terputus di bagian dalam atau tancapannya kurang rapat di breadboard.
* Resistor 220 Ohm yang terhubung ke jalur tersebut mengalami kerusakan.
* Pin digital pada Arduino Uno yang ditugaskan untuk segmen tersebut sudah mati.
* Komponen LED mikro di dalam *seven segment* untuk huruf atau garis tersebut memang sudah terbakar atau putus.
* Memasukkan kabel jumper ke pin yang tidak sesuai dengan petunjuk

Dari sisi perangkat lunak atau *software*, penyebabnya antara lain
* Ada kesalahan memasukkan urutan nomor pin digital pada array `segmentPins` sehingga program mengirimkan sinyal ke jalur kabel yang salah.
* Terjadi kesalahan penulisan angka biner pada array pola `digitPattern`, misalnya nilai yang seharusnya disetel 1 untuk menyala malah tidak sengaja tertulis 0.

---

### 4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai penambahan (increment) dan pengurangan (decrement) pada sistem counter dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

```cpp
#include <Arduino.h>

// Mendaftarkan urutan pin digital untuk segmen a sampai dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};
// Mendaftarkan pin 2 khusus untuk tombol penambahan angka
const int btnUp = 2;
// Mendaftarkan pin 3 khusus untuk tombol pengurangan angka
const int btnDown = 3;

// Membuat tabel pola biner untuk menampilkan angka 0 sampai huruf F
byte digitPattern[16][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, // Pola angka 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // Pola angka 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // Pola angka 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // Pola angka 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // Pola angka 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // Pola angka 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // Pola angka 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // Pola angka 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // Pola angka 8
  {1, 1, 1, 1, 0, 1, 1, 0}, // Pola angka 9
  {1, 1, 1, 0, 1, 1, 1, 0}, // Pola huruf A
  {0, 0, 1, 1, 1, 1, 1, 0}, // Pola huruf b
  {1, 0, 0, 1, 1, 1, 0, 0}, // Pola huruf C
  {0, 1, 1, 1, 1, 0, 1, 0}, // Pola huruf d
  {1, 0, 0, 1, 1, 1, 1, 0}, // Pola huruf E
  {1, 0, 0, 0, 1, 1, 1, 0}  // Pola huruf F
};

// Variabel penyimpan angka saat ini
int currentDigit = 0;
// Variabel status tombol sebelumnya untuk debouncing
bool lastUpState = HIGH;
bool lastDownState = HIGH;

// Fungsi menampilkan angka
void displayDigit(int num) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

// Fungsi setup yang berjalan sekali
void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  
  displayDigit(currentDigit);
}

// Fungsi loop utama
void loop() {
  bool upState = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  // Logika tombol penambahan (increment)
  if (lastUpState == HIGH && upState == LOW) {
    currentDigit++;
    if (currentDigit > 15) currentDigit = 0;
    displayDigit(currentDigit);
    delay(50);
  }
  
  // Logika tombol pengurangan (decrement)
  if (lastDownState == HIGH && downState == LOW) {
    currentDigit--;
    if (currentDigit < 0) currentDigit = 15;
    displayDigit(currentDigit);
    delay(50);
  }
  
  lastUpState = upState;
  lastDownState = downState;
}
