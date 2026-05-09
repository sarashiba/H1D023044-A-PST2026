# Jawaban Pertanyaan Praktikum 5A Multitasking

## 1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! 
Ketiga task tersebut sebenarnya berjalan secara bergantian, bukan bersamaan secara absolut. Hal ini terjadi karena mikrokontroler pada Arduino Uno hanya memiliki satu inti pemrosesan tunggal. Mekanisme peralihan ini diatur langsung oleh sistem scheduler bawaan dari FreeRTOS. Scheduler bertugas melakukan perpindahan eksekusi antar task dengan sangat cepat berdasarkan prioritas dan waktu tunda masing-masing task. Karena proses peralihan antar task ini berlangsung dalam hitungan milidetik, mata manusia menangkapnya seolah-olah ketiga task tersebut berjalan secara bersamaan.

## 2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya! 
Untuk menambahkan task baru ke dalam sistem RTOS, terdapat tiga tahapan utama yang perlu dilakukan. Pertama, mendeklarasikan prototipe fungsi untuk task keempat di bagian awal program sebelum fungsi setup. Kedua, mendaftarkan task tersebut menggunakan fungsi xTaskCreate di dalam fungsi setup tepat sebelum pemanggilan vTaskStartScheduler. Pendaftaran ini mencakup penentuan nama task, ukuran memori, dan tingkat prioritas. Ketiga, membuat blok fungsi utama di bagian bawah program yang berisi instruksi perulangan tanpa henti agar task dapat terus dieksekusi oleh sistem secara berulang.

## 3. Modifikasi Program dengan Sensor Potensiometer
Modifikasi ini memungkinkan kecepatan kedip LED dapat dikontrol secara langsung melalui putaran sensor potensiometer tanpa mengganggu jadwal eksekusi task lainnya.

![Hasil modifikasi](modifikasi_5A.gif)

Berikut merupakan kode program hasil modifikasi

```cpp
#include <Arduino_FreeRTOS.h>

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  Serial.begin(9600);

  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
}

void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while(1) {
    int sensorValue = analogRead(A0);
    int delayTime = map(sensorValue, 0, 1023, 50, 1000);

    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while(1) {
    int sensorValue = analogRead(A0);
    int delayTime = map(sensorValue, 0, 1023, 100, 2000);

    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```

Penjelasan alur kerja program modifikasi
* Pembacaan data sensor dilakukan menggunakan fungsi analogRead pada pin A0 yang diletakkan di dalam perulangan masing-masing task pengontrol LED.
* Nilai mentah dari potensiometer yang berada pada rentang angka 0 hingga 1023 dikonversi menjadi rentang waktu yang aman menggunakan fungsi map. Hal ini bertujuan untuk mencegah nilai delay mencapai angka nol yang dapat menyebabkan satu task memonopoli pemrosesan mikrokontroler.
* Hasil konversi waktu tersebut kemudian digunakan sebagai parameter pada fungsi vTaskDelay. Saat program dijalankan dan potensiometer diputar, nilai jeda waktu akan berubah secara dinamis sehingga kecepatan nyala dan mati LED akan mengikuti besaran nilai potensiometer tersebut.