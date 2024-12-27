#define BLYNK_TEMPLATE_ID "TMPL6xd4lU-gw"
#define BLYNK_TEMPLATE_NAME "Iot Incubator Esp32"
#define BLYNK_AUTH_TOKEN "XXQXlRftzJkW-8RDHg6427wfMkycDB2D"

#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Konfigurasi pin
#define relayFan 5
#define relayLightBulb 14
#define DHTPIN 33
#define DHTTYPE DHT22

// Inisialisasi DHT
DHT dht(DHTPIN, DHTTYPE);

// Konstanta suhu
const float bestTemp = 37.5;
const float tempTolerance = 0.5;
const float maxTolerance = 1.0;

// Variabel global
bool incubatorStatus = true; // Status alat (On/Off)

// Blynk Auth Token
char auth[] = BLYNK_AUTH_TOKEN; // Token autentikasi Blynk
char ssid[] = "Reza";           // Nama WiFi Anda
char pass[] = "rezagacor";      // Password WiFi Anda

void setup() {
  // Inisialisasi pin dan sensor
  pinMode(relayFan, OUTPUT);
  pinMode(relayLightBulb, OUTPUT);
  dht.begin();

  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  Serial.println("Program dimulai...");

  // Inisialisasi Blynk
  Blynk.begin(auth, ssid, pass);
}

// Fungsi untuk membaca input dari Blynk
BLYNK_WRITE(V1) {
  incubatorStatus = param.asInt(); // Membaca status dari Blynk (1: On, 0: Off)
}

void loop() {
  // Menjalankan proses Blynk
  Blynk.run();

  if (incubatorStatus) {
    // Membaca suhu dan kelembapan dari DHT22
    float temperature = dht.readTemperature(); // Membaca suhu dalam Celcius
    float humidity = dht.readHumidity();       // Membaca kelembapan

    // Periksa apakah pembacaan valid
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      delay(2000); // Tunggu sebelum mencoba membaca lagi
      return;
    }

    // Cetak suhu dan kelembapan di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Kelembapan: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Kirim data ke Blynk
    Blynk.virtualWrite(V5, temperature); // Kirim suhu ke Virtual Pin V5
    Blynk.virtualWrite(V6, humidity);    // Kirim kelembapan ke Virtual Pin V6

    // Logika pengaturan kipas dan lampu
    if (temperature < bestTemp - tempTolerance) {
      LightOn();
      fanOFF();
      Serial.println("Menyalakan lampu, mematikan kipas.");
    } else if (temperature > bestTemp + maxTolerance) {
      LightOff();
      fanOn();
      Serial.println("Mematikan lampu, menyalakan kipas.");
    } else {
      LightOff();
      fanOFF();
      Serial.println("Suhu stabil, lampu dan kipas mati.");
    }
  } else {
    // Jika alat dimatikan
    fanOFF();
    LightOff();
    Serial.println("Inkubator dimatikan.");
  }

  // Tambahkan delay agar tidak terlalu cepat membaca sensor
  delay(2000);
}

// Fungsi kontrol kipas
void fanOn() {
  digitalWrite(relayFan, HIGH);
}

void fanOFF() {
  digitalWrite(relayFan, LOW);
}

// Fungsi kontrol lampu
void LightOn() {
  digitalWrite(relayLightBulb, HIGH);
}

void LightOff() {
  digitalWrite(relayLightBulb, LOW);
}
