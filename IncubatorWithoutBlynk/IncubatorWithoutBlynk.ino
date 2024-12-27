#include <DHT.h>

#define relayFan 5
#define relayLightBulb 14 
#define DHTPIN 33
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Konstanta suhu
const float bestTemp = 37.5;
const float tempTolerance = 0.5;
const float maxTolerance = 2.5;

// Variabel global
bool incubatorStatus = true;  // Status alat (On/Off)

void setup() {
  // Inisialisasi pin dan sensor
  pinMode(relayFan, OUTPUT);
  pinMode(relayLightBulb, OUTPUT);
  dht.begin();

  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  Serial.println("Program dimulai...");
}

void loop() {
  // Cek status inkubator
  if (incubatorStatus) {
    // Membaca suhu dari DHT22
    float temperature = dht.readTemperature(); // Membaca suhu dalam Celcius

    if (isnan(temperature)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      delay(2000); // Tunggu sebelum mencoba membaca lagi
      return;
    }

    // Tampilkan suhu di Serial Monitor
    Serial.print("Suhu: ");
    Serial.println(temperature);

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
