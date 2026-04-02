# Jawaban Pertanyaan Praktikum Modul 1 Perulangan

---

### 1. Gambarkan rangkaian schematic 5 LED running yang digunakan pada percobaan!
<img width="867" height="690" alt="Image" src="https://github.com/user-attachments/assets/7197a6d8-3129-4dd6-b333-2ff68eec6782" />

---

### 2. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan!
Program menggunakan perulangan `for` untuk menghitung maju dari pin terendah ke tertinggi, yaitu `for (int ledPin = 2; ledPin <= 6; ledPin++)`. Di dalam perulangan tersebut, program akan menghidupkan LED dengan perintah `digitalWrite(ledPin, HIGH)`, menahannya sesaat menggunakan `delay(timer)`, lalu mematikan LED tersebut dengan `digitalWrite(ledPin, LOW)`. Proses ini diulang terus-menerus dari kiri hingga pin ke-6 di paling kanan sehingga menciptakan efek cahaya berjalan menyamping.

---

### 3. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri!
Setelah perulangan pertama selesai, program mengeksekusi perulangan `for` yang kedua untuk menghitung mundur dari pin tertinggi ke terendah, yaitu `for (int ledPin = 6; ledPin >= 2; ledPin--)`. LED pada pin yang sedang diakses akan dihidupkan, diberi jeda waktu, kemudian dimatikan sebelum perulangan berpindah ke pin yang angkanya lebih kecil. Logika ini menciptakan efek cahaya yang berjalan terbalik ke arah asalnya.

---

### 4. Buatkan program agar LED menyala tiga LED kanan dan tiga LED kiri secara bergantian dan berikan penjelasan disetiap baris kode nya!

#### Kode Program
```cpp
int timer = 500;

void setup() {
  // Mengatur Pin 2 sampai Pin 6 sebagai OUTPUT
  for (int pin = 2; pin <= 6; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // Fase 1: Menyalakan 3 LED Kiri (Pin 2, 3, 4)
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  delay(timer);

  // Fase 2: Menyalakan 3 LED Kanan (Pin 4, 5, 6)
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  delay(timer);
}
