# Jawaban Pertanyaan Praktikum UART
---

## 1. Jelaskan proses dari input keyboard hingga LED menyala/mati! 
Proses komunikasi dari input hingga aksi perangkat keras berjalan melalui tahapan berikut:
1. Pengguna mengetikkan karakter (contoh: '1' atau '0') di Serial Monitor (komputer) lalu menekan tombol Send/Enter.
2. Komputer mengirimkan karakter tersebut dalam format kode ASCII melalui kabel USB menuju papan Arduino.
3. Chip *converter* USB-to-Serial pada papan Arduino menerjemahkan paket data USB menjadi sinyal serial TTL standar dan meneruskannya ke pin RX (Receive) mikrokontroler utama (ATmega328P).
4. Karakter ASCII yang masuk akan disimpan sementara di dalam memori *buffer* serial Arduino.
5. Pada program (sketsa), fungsi `Serial.available()` mendeteksi adanya data di dalam *buffer*.
6. Fungsi `Serial.read()` mengambil satu karakter dari *buffer* tersebut dan menyimpannya dalam variabel.
7. Mikrokontroler mengevaluasi variabel tersebut menggunakan struktur kontrol `if-else`.
8. Jika karakter adalah '1', perintah `digitalWrite(PIN_LED, HIGH)` dikirim, membuka gerbang logika sehingga tegangan 5V mengalir ke pin, dan LED menyala. Jika karakter '0', `digitalWrite(PIN_LED, LOW)` memutus tegangan menjadi 0V, dan LED mati.

---

## 2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan? 
**Alasan penggunaan:** `Serial.available()` digunakan sebagai "penjaga gerbang" untuk memastikan bahwa ada data yang benar-benar siap dibaca di dalam *buffer* serial. Fungsi ini mengembalikan nilai jumlah *byte* (karakter) yang sedang mengantre.

**Jika dihilangkan:** Mikrokontroler mengeksekusi blok kode `loop()` dengan sangat cepat (ribuan kali per detik). Jika pengecekan `Serial.available()` dihilangkan, fungsi `Serial.read()` akan terus-menerus membaca meskipun pengguna tidak mengetikkan apa-apa di Serial Monitor. Ketika *buffer* kosong, `Serial.read()` akan mengembalikan angka `-1` (atau *garbage value*). Akibatnya, Arduino memproses data kosong yang menyebabkan logika program menjadi kacau dan mikrokontroler bekerja sangat tidak efisien.

---

## 3. Modifikasi Program (LED Blink input '2')
Hasil Modifikasi:
![Hasil modifikasi](modifikasi_UART.gif)

**Kode Program:**
```cpp
#include <Arduino.h>

const int PIN_LED = 8;
char command = '0'; // Variabel penyimpan status terakhir

void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1': ON, '0': OFF, '2': BLINK");
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char data = Serial.read();
    
    // Validasi data (mengabaikan karakter enter/newline)
    if (data == '1' || data == '0' || data == '2') {
      command = data; // Update status terbaru
      Serial.print("Status diubah ke: ");
      Serial.println(command);
    } else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // Eksekusi aksi berdasarkan status terbaru
  if (command == '1') {
    digitalWrite(PIN_LED, HIGH);
  } 
  else if (command == '0') {
    digitalWrite(PIN_LED, LOW);
  } 
  else if (command == '2') {
    digitalWrite(PIN_LED, HIGH);
    delay(300); 
    digitalWrite(PIN_LED, LOW);
    delay(300);
  }
}
```
## Penjelasan Kode (Modifikasi UART Blink)

1. `#include <Arduino.h>`: Memanggil pustaka utama atau dasar milik sistem Arduino.
2. `const int PIN_LED = 8;`: Mendeklarasikan konstanta bernama `PIN_LED` di Pin Digital 8 sebagai jalur terhubungnya lampu LED.
3. `char command = '0';`: Membuat variabel global bernama `command` dengan tipe data karakter (`char`). Nilai awalnya diset `'0'` (mati). Variabel ini sangat penting karena berfungsi sebagai memori untuk "mengingat" status perintah terakhir yang diketik oleh pengguna.
4. `void setup() {`: Membuka fungsi inisialisasi yang hanya dieksekusi satu kali saat Arduino pertama kali diberi daya atau di-reset.
5. `Serial.begin(9600);`: Mengaktifkan jalur komunikasi serial antara Arduino dan komputer dengan kecepatan transfer data (*baud rate*) 9600 bit per detik.
6. `Serial.println(...);`: Mencetak teks panduan atau instruksi penggunaan program ke layar Serial Monitor.
7. `pinMode(PIN_LED, OUTPUT);`: Mengatur fungsi Pin 8 menjadi jalur keluaran (*output*) agar bisa mengirimkan tegangan listrik ke LED.
8. `void loop() {`: Membuka fungsi utama yang akan dieksekusi secara berulang-ulang tanpa henti selama Arduino menyala.
9. `if (Serial.available() > 0) {`: Melakukan pengecekan apakah ada data ketikan dari *keyboard* yang sudah masuk dan mengantre di dalam memori sementara (*buffer* serial).
10. `char data = Serial.read();`: Membaca satu karakter dari memori *buffer* tersebut dan menyimpannya ke dalam variabel sementara bernama `data`.
11. `if (data == '1' || data == '0' || data == '2') {`: Memvalidasi input, yaitu mengecek apakah karakter yang baru dibaca adalah angka yang sah ('1', '0', atau '2').
12. `command = data;`: Jika input tersebut sah, maka perbarui (timpa) nilai variabel status `command` dengan data yang baru. Hal ini akan mengubah mode kerja sistem di tahap selanjutnya.
13. `Serial.print(...)` / `Serial.println(command);`: Memberikan teks respons atau *feedback* ke Serial Monitor bahwa perintah sudah berhasil diterima dan mode telah diubah.
14. `} else if (data != '\n' && data != '\r') {`: Fungsi filter untuk mengabaikan karakter kontrol tersembunyi seperti penekanan tombol 'Enter' (Newline `\n` dan Carriage Return `\r`).
15. `Serial.println("Perintah tidak dikenal");`: Menampilkan pesan *error* jika pengguna mengetikkan huruf atau angka selain 1, 0, dan 2.
16. `if (command == '1') {`: Mengecek apakah memori status saat ini adalah mode '1'.
17. `digitalWrite(PIN_LED, HIGH);`: Jika ya, berikan tegangan 5V ke Pin 8 agar LED menyala secara konstan.
18. `else if (command == '0') {`: Mengecek apakah memori status saat ini adalah mode '0'.
19. `digitalWrite(PIN_LED, LOW);`: Jika ya, putus tegangan ke Pin 8 (menjadi 0V) agar LED mati secara konstan.
20. `else if (command == '2') {`: Mengecek apakah memori status saat ini adalah mode '2' (Blink).
21. `digitalWrite(PIN_LED, HIGH); delay(300);`: Jika ya, nyalakan LED, lalu hentikan sementara program (*delay*) selama 300 milidetik agar nyala LED bisa terlihat.
22. `digitalWrite(PIN_LED, LOW); delay(300);`: Matikan LED, lalu hentikan sementara program selama 300 milidetik. *Catatan: Karena blok ini berada di dalam fungsi `loop()`, siklus nyala-mati ini akan terus berulang secara otomatis selama nilai variabel `command` tidak diubah oleh input baru.*

---

## 4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem
Pada modifikasi program di atas, digunakan fungsi `delay()`. 

**Pengaruhnya terhadap sistem:**
Fungsi `delay()` bersifat **blocking**. Saat mikrokontroler mengeksekusi `delay(300)`, sistem benar-benar "terhenti/tidur" selama 300 milidetik dan tidak bisa menjalankan instruksi baris lain di belakangnya, termasuk memeriksa kode `Serial.available()`. 

Dampaknya Saat kita sedang berada pada mode Blink (command = '2'), LED memakan waktu total 600 milidetik untuk satu siklus berkedip. Jika pengguna mengetikkan '0' tepat saat sistem sedang tidur, Arduino mungkin membutuhkan waktu hingga setengah detik lebih untuk menyadari input tersebut dan mematikan LED. Sistem menjadi kurang responsif.

Jika ingin membangun sistem protokol komunikasi yang profesional, responsif secara instan, dan mendukung proses ganda (*multitasking*), jauh lebih disarankan untuk menggunakan algoritma **`millis()`** karena sifatnya yang **non-blocking** (waktu dicek tanpa menghentikan jalan utama program).