#include "file_code_parameter.h"
#include "file_code_web.h"
#include "file_code_network.h"
#include "file_code_server.h"
#include "file_code_ota.h"
#include "file_code_inout.h"
#include "file_code_proses.h"


void setup() {
  // Inisial Serial.
  initSerial();

  // Inisial SW-420.
  setup_sw420();

  // Inisial Buzzer.
  setup_buzzer();

  // Fungsi Koneksi WIFI.
  initWiFiConnection();
}

void loop() {
  // Fungsi ESP Handle Server.
  loopESPServer();

  // Fungsi Loop SW420.
  loop_sw420();

  // Fungsi Loop Buzzer.
  updateBuzzer();

  // Fungsi set alarm buzzer.
  if( dangerReported == true && isMute == false && isVibrating == true){
    buzzerAktif = true ;
  } 

  // Pemeriksaan Koneksi WIFI.
  if( isDeviceConnectWifi() == true){
    if( isTimerExpired() )
      stat_wifi = "Disconnect"     ;
    else
      stat_wifi = "Connect Client" ;
  }
  else stat_wifi = "Status:No WIFI" ;

  delay(10);
}
