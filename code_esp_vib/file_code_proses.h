// Fungsi untuk menginisialisasi serial hardware esp.
void initSerial(){
  Serial.begin(115200);
  delay(1000); 
  for( int i = 0 ; i < 10 ; i++ ){
    Serial.println();
    delay(100);
  }
}

// Fungsi untuk menginisialisasi koneksi WiFi
void initWiFiConnection(){
  if( searchSSIDWifi(WIFI_SSID) == true ){
    if( koneksi_Wifi() ){
      setup_ota();
      setup_server();
      server.begin();
    }else{
      while(true){delay(10);}
    }
  }else{
    while(true){delay(10);}
  }
}

// Fungsi loop utama untuk ESP8266 Web Server dan OTA
void loopESPServer(){
  ota_code_loop();        // Menjalankan proses OTA (Over-The-Air update).
  server.handleClient();  // Menangani permintaan (request) HTTP dari client.
}