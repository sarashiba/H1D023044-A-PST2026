# Jawaban Pertanyaan Praktikum 5B Komunikasi Task

## 1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Secara prinsip dasar RTOS mengeksekusi kedua task secara bergantian melalui mekanisme context switching. Scheduler dari FreeRTOS bertugas mengatur perpindahan alokasi prosesor dari task pembacaan sensor ke task penampil layar dengan kecepatan tinggi. Karena proses pergantian ini terjadi dalam hitungan milidetik, sistem secara kasat mata terlihat seolah-olah menjalankan kedua task tersebut secara berbarengan.

## 2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
Program ini sangat aman dan tidak berpotensi mengalami race condition. Alasannya karena kita menggunakan fitur Queue bawaan FreeRTOS sebagai jalur komunikasi antar task. Sistem antrean pada Queue sudah dirancang secara thread-safe. Saat task pengirim sedang memasukkan data ke dalam antrean, kernel FreeRTOS akan mengamankan aksesnya sehingga task penerima tidak akan membaca data yang bentuknya belum sempurna. Data tersebut baru bisa ditarik jika proses pengirimannya sudah benar-benar tuntas seratus persen.

## 3. Hasil Modifikasi Program dengan Sensor DHT22
Modifikasi program dilakukan dengan mengintegrasikan sensor DHT22 untuk menampilkan informasi suhu dan kelembapan secara dinamis. Namun, pada pelaksanaannya, perangkat keras yang digunakan harus dialihkan dari Arduino Uno menjadi mikrokontroler ESP32.

**Alasan Penggunaan ESP32:**
Pustaka eksternal sensor DHT membutuhkan kapasitas memori SRAM yang cukup besar untuk melakukan komputasi tipe data desimal (*float*). Arduino Uno hanya memiliki 2 KB SRAM, sehingga saat digabungkan dengan alokasi memori untuk sistem antrean dan *scheduler* dari FreeRTOS, mikrokontroler mengalami kehabisan memori (*stack overflow*) yang berujung pada *crash* atau *bootloop*. Oleh karena itu, pengujian dialihkan ke *board* ESP32 yang memiliki memori SRAM jauh lebih lega (520 KB) dan sudah terintegrasi dengan sistem RTOS secara bawaan, sehingga program dapat berjalan dengan sangat stabil.

![Hasil modifikasi](modifikasi_5B.gif)

**Kode Program Modifikasi (ESP32):**
```cpp
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

struct readings {
  float temp;
  float h;
};

QueueHandle_t my_queue;

void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(115200);
  dht.begin();

  my_queue = xQueueCreate(5, sizeof(struct readings));

  xTaskCreate(read_data, "baca_sensor", 2048, NULL, 1, NULL);
  xTaskCreate(display, "tampil_data", 2048, NULL, 1, NULL);

  Serial.println("Program ESP32 mulai berjalan...");
}

void loop() {
}

void read_data(void *pvParameters) {
  struct readings x;

  vTaskDelay(2000 / portTICK_PERIOD_MS);

  for(;;) {
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();

    xQueueSend(my_queue, &x, portMAX_DELAY);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters) {
  struct readings x;

  for(;;) {
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("Suhu = ");
      Serial.print(x.temp);
      Serial.println(" *C");

      Serial.print("Kelembapan = ");
      Serial.print(x.h);
      Serial.println(" %");
      Serial.println("--------------------");
    }
  }
}
```

**Penjelasan Alur Program Modifikasi:**
* Tipe data pada struktur penampung diubah menjadi `float` agar dapat menyimpan nilai desimal suhu dan kelembapan dengan akurat.
* Task `read_data` bertugas mengambil variabel suhu dan kelembapan langsung dari pin sensor menggunakan perintah dari pustaka DHT, lalu mengirimkan paket struktur data tersebut ke dalam antrean memori menggunakan instruksi `xQueueSend`.
* Task `display` bertugas memantau antrean menggunakan instruksi `xQueueReceive` dan akan langsung memecah paket data tersebut untuk dicetak ke Serial Monitor setiap kali ada kiriman baru.