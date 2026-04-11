# Jawaban Pertanyaan Praktikum Seven Segment
### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
<img width="913" height="713" alt="Screenshot 2026-04-11 134755" src="https://github.com/user-attachments/assets/a414d274-1f23-4f12-b482-a0825837f4e2" />

---

### 2. Apa yang terjadi jika nilai num lebih dari 15?

Jika nilai variabel `num` melebihi angka 15, program akan mengalami masalah pembacaan memori atau biasa disebut *Out of Bounds Array*. Array `digitPattern` yang menyusun angka pada program kita secara spesifik hanya dideklarasikan memiliki 16 baris (yaitu indeks dari 0 sampai 15). 

Ketika fungsi pemanggil dipaksa mengeksekusi angka 16 atau lebih, mikrokontroler akan kebingungan dan membaca alamat memori acak di luar rentang array tersebut. Efek langsung yang akan terjadi di dunia nyata adalah lampu LED pada 7-segment akan menyala secara tidak beraturan membentuk simbol acak, atau dalam skenario terburuk, program Arduino akan mengalami *crash* dan mikrokontroler akan melakukan *restart* dengan sendirinya.

---

### 3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan alasanya!

Program dan rangkaian pada percobaan ini menggunakan konfigurasi **Common Anode**. 

Alasannya adalah Pertama, pada sisi *hardware*, pin tengah (*common*) pada *seven segment* dihubungkan langsung ke sumber tegangan positif (5V). Pada sistem anoda umum, seluruh kutub positif LED disatukan, sehingga untuk menyalakan lampu, pin digital Arduino harus bertindak sebagai *ground* dengan memberikan beda potensial berupa sinyal logika `LOW` (atau 0). 

Kedua, dari sisi *software*, terdapat penggunaan operator negasi `!` pada fungsi pemanggil `!digitPattern[num][i]`. Array asli `digitPattern` direpresentasikan dengan angka `1` untuk kondisi *ON*, namun karena rangkaian menggunakan *Common Anode* yang butuh sinyal `0` untuk menyala, operator `!` digunakan untuk membalik logika arus `HIGH` menjadi `LOW` dengan tepat.

---

### 4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

```cpp
#include <Arduino.h>

// Pin mapping untuk segmen a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Pola biner untuk angka 0-9 dan huruf A-F
// 1 = Segmen ON, 0 = Segmen OFF
byte digitPattern[16][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8
  {1, 1, 1, 1, 0, 1, 1, 0}, // 9
  {1, 1, 1, 0, 1, 1, 1, 0}, // A
  {0, 0, 1, 1, 1, 1, 1, 0}, // b
  {1, 0, 0, 1, 1, 1, 0, 0}, // C
  {0, 1, 1, 1, 1, 0, 1, 0}, // d
  {1, 0, 0, 1, 1, 1, 1, 0}, // E
  {1, 0, 0, 0, 1, 1, 1, 0}  // F
};

// Fungsi untuk menampilkan digit ke 7-segment
void displayDigit(int num) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup() {
  // Mengatur semua pin segmen sebagai OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop() {
  // Menghitung mundur dari 15 (F) menuju 0
  for (int i = 15; i >= 0; i--) {
    displayDigit(i);
    delay(1000); // Jeda 1 detik per digit
  }
}
