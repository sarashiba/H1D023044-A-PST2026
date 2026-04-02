# Jawaban Pertanyaan Praktikum Modul 1 Percabangan

---

### 1. Pada kondisi apa program masuk ke blok if?
Program mengeksekusi blok `if` pada kondisi variabel `timeDelay` bernilai kurang dari atau sama dengan 100. Kondisi ini tercapai saat siklus kedipan LED berada pada fase yang paling cepat.

---

### 2. Pada kondisi apa program masuk ke blok else?
Program masuk ke blok `else` apabila kondisi pada blok `if` belum terpenuhi, yaitu ketika variabel `timeDelay` masih bernilai lebih besar dari 100. Di dalam blok ini program melakukan pengurangan nilai `timeDelay` sebanyak 100 secara terus-menerus agar efek kedipannya bertahap menjadi semakin cepat.

---

### 3. Apa fungsi dari perintah delay(timeDelay)?
Perintah `delay(timeDelay)` berfungsi memberikan waktu jeda sesaat bagi mikrokontroler sebelum mengeksekusi baris kode selanjutnya. Penggunaan variabel `timeDelay` di dalam kurung membuat lamanya waktu jeda dapat dimanipulasi secara dinamis.

---

### 4. Modifikasi Alur LED beserta Penjelasan Baris Kode
Berikut merupakan pembaruan kode agar program berubah dari cepat menuju sedang lalu mati.

#### Kode Program
```cpp
const int ledPin = 13;
int timeDelay = 1000;
int phase = 0; // 0 untuk fase makin cepat, 1 untuk fase sedang, 2 untuk fase mati

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (phase == 0) {
    digitalWrite(ledPin, HIGH);
    delay(timeDelay);
    digitalWrite(ledPin, LOW);
    delay(timeDelay);

    timeDelay -= 100;

    if (timeDelay <= 100) {
      phase = 1;
      timeDelay = 500;
    }
  } else if (phase == 1) {
    digitalWrite(ledPin, HIGH);
    delay(timeDelay);
    digitalWrite(ledPin, LOW);
    delay(timeDelay);

    phase = 2;
  } else if (phase == 2) {
    digitalWrite(ledPin, LOW);
    delay(3000);

    timeDelay = 1000;
    phase = 0;
  }
}
```

#### Penjelasan Baris Kode
- `int phase = 0;` merupakan inisialisasi variabel untuk melacak tahapan kedipan LED.
- `if (phase == 0)` merupakan pengecekan apakah program sedang berada pada tahap mempercepat kedipan.
- `timeDelay -= 100;` berguna mengurangi nilai jeda sebanyak 100 milidetik agar lampu berkedip lebih cepat pada perulangan berikutnya.
- `if (timeDelay <= 100)` bertugas mendeteksi saat kedipan sudah mencapai titik paling cepat. Jika terpenuhi, program mengubah fase menjadi 1 dan mengatur ulang `timeDelay` menjadi 500 milidetik untuk kecepatan sedang.
- `else if (phase == 1)` merupakan blok perintah yang akan dijalankan satu kali untuk menampilkan efek kedipan dengan kecepatan sedang, lalu memindahkan status program ke fase 2.
- `else if (phase == 2)` merupakan blok perintah di mana lampu LED dimatikan sepenuhnya menggunakan `digitalWrite(ledPin, LOW)`. Setelah jeda beberapa detik berlalu, program mereset variabel `timeDelay` ke 1000 milidetik dan mengembalikan `phase` ke angka 0 untuk mengulangi seluruh proses dari awal.
