//----------------| Code SW-420 - START - |------------------//

// Definisi Pin
const int SENSOR_PIN = D2; 

// Variabel untuk Interrupt dan Timing
volatile bool vibrationDetected = false;

// Konstanta Konfigurasi
const unsigned long TIMEOUT_MS = 4000;      // Jeda untuk menyatakan getaran benar-benar berhenti
const unsigned long IGNORE_THRESHOLD = 4000; // Getaran < 4 detik diabaikan (salah pembacaan)
const unsigned long DANGER_THRESHOLD = 8000; // Getaran > 7 detik dianggap Bahaya

// IRAM_ATTR diperlukan untuk fungsi interrupt pada ESP8266
void IRAM_ATTR handleVibration() {
  vibrationDetected = true;
}

// Kode inisial SW-420.
void setup_sw420() {
  // Set pin input.
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  
  // Memasang Interrupt pada perubahan sinyal
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), handleVibration, FALLING);
}

// Fungsi untuk menentukan hasil akhir setelah getaran berhenti
void finalizeStatus(unsigned long totalDuration) {
  isVibrating = false;
  
  // Koreksi durasi (dikurangi timeout deteksi)
  unsigned long actualDuration = (totalDuration > TIMEOUT_MS) ? (totalDuration - TIMEOUT_MS) : 0;

  if (actualDuration < IGNORE_THRESHOLD) {
    // Jika getaran berhenti sangat cepat (< 2 detik)
    Serial.println("\n[Info] Getaran singkat terdeteksi. Status: Salah Pembacaan (Diabaikan).");
  } else {
    Serial.print("\nGetaran Berhenti. Total Durasi Aktif: ");
    Serial.print(actualDuration / 1000.0);
    Serial.println(" detik.");
    
    if (actualDuration >= DANGER_THRESHOLD) {
      Serial.println("Status Akhir: BAHAYA (Telah Berlalu)");
    } else {
      Serial.println("Status Akhir: WASPADA (Telah Berlalu)");
    }

    waspadaReported = false;
    dangerReported = false;
  }
  
  Serial.println("------------------------------------");
  Serial.println("Status: Standby Aman");
}


void loop_sw420() {
  unsigned long currentTime = millis();

  // 1. Deteksi Awal Getaran
  if (vibrationDetected) {
    if (!isVibrating) {
      isVibrating = true;
      vibrationStartTime = currentTime;
      waspadaReported = false;
      dangerReported = false;
    }
    lastPulseTime = currentTime;
    vibrationDetected = false; 
  }

  // 2. Monitoring Selama Getaran Berlangsung
  if (isVibrating) {
    unsigned long duration = currentTime - vibrationStartTime;

    // Status: WASPADA (Jika getaran sudah lebih dari 2 detik)
    if (duration >= IGNORE_THRESHOLD && !waspadaReported) {
      Serial.println("\n[!] Status: WASPADA (Pintu bergetar > 2s)");
      waspadaReported = true;
    }

    // Status: BAHAYA (Jika getaran berlanjut hingga lebih dari 5 detik)
    if (duration >= DANGER_THRESHOLD && !dangerReported) {
      Serial.println("STATUS: >>> BAHAYA! (Upaya paksa/Getaran > 5s) <<<");
      dangerReported = true; 
    }

    // 3. Deteksi Getaran Berhenti (Berdasarkan Timeout)
    if (currentTime - lastPulseTime > TIMEOUT_MS) {
      finalizeStatus(duration);
    }
  }
}




//----------------| Code SW-420 - END - |------------------//




//----------------| Code Buzzer - START - |------------------//
// Pin Definitions
const int BUZZER_PIN = D5; // Silakan sesuaikan dengan pin yang Anda gunakan (D5 = GPIO14)



// Definisi Nada (Frekuensi dalam Hz)
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262

// Melodi sederhana
int melodi[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// Durasi nada (4 = seperempat ketukan, 8 = seperdelapan ketukan, dst)
int durasiNada[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// Variabel Kontrol Non-Blocking
unsigned long waktuSebelumnya = 0;
int indeksNada = 0;
long jedaNada = 0;
bool sedangBunyi = false;

/**
 * Fungsi untuk mengelola logika buzzer tanpa blocking
 */
void updateBuzzer() {
  unsigned long waktuSekarang = millis();

  if (buzzerAktif) {
    // Jika sedang tidak ada nada yang berbunyi dan waktu jeda sudah lewat
    if (!sedangBunyi && (waktuSekarang - waktuSebelumnya >= jedaNada)) {
      
      // Hitung durasi nada dalam milidetik
      // (1000ms / tipe nada), misal 1000/4 = 250ms
      int durasi = 1000 / durasiNada[indeksNada];
      
      if (melodi[indeksNada] != 0) {
        tone(BUZZER_PIN, melodi[indeksNada], durasi);
      } else {
        noTone(BUZZER_PIN);
      }

      waktuSebelumnya = waktuSekarang;
      jedaNada = durasi * 1.30; // Berikan jeda 30% antar nada agar terdengar terpisah
      sedangBunyi = true;
    }

    // Jika nada sudah selesai durasinya, lanjut ke indeks berikutnya
    if (sedangBunyi && (waktuSekarang - waktuSebelumnya >= (1000 / durasiNada[indeksNada]))) {
      sedangBunyi = false;
      indeksNada++;
      
      // Reset melodi jika sudah sampai ujung
      if (indeksNada >= sizeof(melodi) / sizeof(int)) {
        indeksNada = 0;
      }
    }
  } else {
    // Jika pemicu false, matikan buzzer dan reset posisi melodi
    noTone(BUZZER_PIN);
    indeksNada = 0;
    sedangBunyi = false;
    waktuSebelumnya = 0;
  }
}

void setup_buzzer(){
  pinMode(BUZZER_PIN, OUTPUT);
}
//----------------| Code Buzzer - END - |------------------//  


// Fungsi Endpoint
String generateDataEndPoint(){
  return String(isVibrating,DEC) + "x" + String(waspadaReported,DEC) + "x" + String(dangerReported,DEC) + "x" + String(isMute,DEC) + "x" + String(buzzerAktif,DEC) + "x#" ;
}