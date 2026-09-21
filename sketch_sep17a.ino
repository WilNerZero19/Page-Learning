// ===== KONFIGURASI PIN =====

// Pin segmen (A, B, C, D, E, F, G, DP)
const int segmentPins[8] = {2, 3, 4, 5, 6, 8, 9, 10};

// Pin common tiap digit (digit1, digit2, digit3, digit4)
const int digitPins[4] = {11, 12, 13, A0};

// Pin tombol push
const int buttonPin = A1;

// ===== POLA ANGKA 0-9 (Common Cathode: 1 = nyala, 0 = mati) =====
// urutan: a, b, c, d, e, f, g, dp
byte angkaPola[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

// ===== VARIABEL PROGRAM =====
int angkaSekarang = 0;      // angka yang ditampilkan (0-9999)

// untuk debounce tombol
int statusTombolSebelumnya = HIGH;
unsigned long waktuTerakhirTekan = 0;
const unsigned long debounceDelay = 50;

void setup() {
  // set semua pin segmen sebagai output
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // set semua pin common sebagai output
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW); // matikan dulu semua digit (asumsi common cathode)
  }

  // tombol pakai internal pull-up, jadi kondisi normal HIGH, saat ditekan jadi LOW
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  cekTombol();
  tampilkanAngka(angkaSekarang);
}

// ===== FUNGSI CEK TOMBOL (dengan debounce) =====
void cekTombol() {
  int statusTombol = digitalRead(buttonPin);

  // Deteksi transisi dari HIGH ke LOW (tombol baru ditekan)
  if (statusTombol == LOW && statusTombolSebelumnya == HIGH) {
    if (millis() - waktuTerakhirTekan > debounceDelay) {
      angkaSekarang++;           // tambah 1 setiap ditekan
      if (angkaSekarang > 9999) {
        angkaSekarang = 0;       // reset ke 0 kalau sudah lewat 9999
      }
      waktuTerakhirTekan = millis();
    }
  }

  statusTombolSebelumnya = statusTombol;
}

// ===== FUNGSI TAMPILKAN ANGKA 4 DIGIT (multiplexing) =====
void tampilkanAngka(int angka) {
  int digit[4];
  digit[0] = (angka / 1000) % 10; // ribuan
  digit[1] = (angka / 100) % 10;  // ratusan
  digit[2] = (angka / 10) % 10;   // puluhan
  digit[3] = angka % 10;          // satuan

  for (int i = 0; i < 4; i++) {
    // matikan semua digit dulu
    for (int d = 0; d < 4; d++) {
      digitalWrite(digitPins[d], LOW);
    }

    // set pola segmen untuk digit ke-i
    for (int s = 0; s < 8; s++) {
      digitalWrite(segmentPins[s], angkaPola[digit[i]][s]);
    }

    // nyalakan hanya digit ke-i
    digitalWrite(digitPins[i], HIGH);

    delay(3); // waktu nyala tiap digit (multiplexing, jangan terlalu lama)
  }
}