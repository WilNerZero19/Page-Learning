#include "SevSeg.h"

SevSeg sevseg; // Membuat objek SevSeg

unsigned long waktuSebelumnya = 0;
const long interval = 500; // Kecepatan hitung (dalam milidetik)
int hitungan = 1;         // Angka awal mulai dari 1
int berakhir = 9999;

void setup() {
  // --- KONFIGURASI HARDWARE ---
  byte numDigits = 4;                           // Jumlah digit (4 digit)
  byte digitPins[] = {2, 3, 4, 5};              // Pin Arduino untuk kontrol Digit 1, 2, 3, 4
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // Pin Arduino untuk segmen A, B, C, D, E, F, G, DP

  bool resistorsOnSegments = false; // 'false' jika resistor dipasang di pin digit, 'true' jika di pin segmen
  
  // UBAH BAGIAN INI sesuai tipe 7-Segment Anda:
  // COMMON_CATHODE (Katoda) atau COMMON_ANODE (Anoda)
  byte hardwareConfig = COMMON_CATHODE; 
  
  bool updateWithDelays = false;     // Sangat direkomendasikan false
  bool leadingZeros = false;         // 'true' jika ingin angka tampil seperti 0001, 'false' untuk 1
  bool disableDecPoint = true;       // 'true' jika tidak menggunakan titik desimal

  // Memulai library dengan konfigurasi di atas
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  
  sevseg.setBrightness(90); // Mengatur kecerahan layar (0-100)
}

void loop() {
  unsigned long waktuSekarang = millis();

  // Logika menghitung maju dari 1 sampai 9999 tanpa mengganggu multiplexing
  if (waktuSekarang - waktuSebelumnya >= interval) {
    waktuSebelumnya = waktuSekarang;
    
    sevseg.setNumber(hitungan); // Kirim angka ke segment
    
    hitungan++;
    if (hitungan > 9999) {
      hitungan = 1; // Reset kembali ke 1 jika sudah melewati 9999
      
    }
  }

  // Wajib dipanggil terus-menerus agar efek multiplexing bekerja (layar tidak berkedip)
  sevseg.refreshDisplay(); 
}