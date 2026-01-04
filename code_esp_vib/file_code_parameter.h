//
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>
//
#include "ACS712.h"

// Parameter setting wifi.
#define WIFI_SSID       "sakha"
#define WIFI_PASSWORD   "prime468328"

// Buat objek server di port 80
ESP8266WebServer server(80);

// Variabel Status.
String stat_wifi    = "" ;
String stat_sensor  = "" ;

// Variabel status alarm on or mute
bool isMute = false ;

// Variabel status vibrasi sensor.
unsigned long vibrationStartTime = 0;
unsigned long lastPulseTime = 0;
bool isVibrating      = false;
bool waspadaReported  = false;
bool dangerReported   = false;

// Variabel Timer Koneksi Client Web.
unsigned long lastAccessTime  = 0     ;   // menyimpan waktu terakhir akses (ms)
unsigned long timerDuration   = 8000  ;   // contoh durasi 30 detik




String generateDataEndPoint();

// Variabel Global Pemicu -> Setting Buzzer On/Off.
bool buzzerAktif = false;