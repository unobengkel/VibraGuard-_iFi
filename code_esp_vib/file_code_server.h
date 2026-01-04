// Fungsi reset timer (panggil saat user akses endpoint)
void resetTimer() {
  lastAccessTime = millis();
}

// Fungsi cek sisa waktu sejak terakhir akses
unsigned long getElapsedTime() {
  return millis() - lastAccessTime;
}

// Fungsi cek apakah timer sudah lewat
bool isTimerExpired() {
  return getElapsedTime() >= timerDuration;
}


void handleRoot() {
  resetTimer();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", htmlPage );
}

void handleGet() {
  resetTimer();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", generateDataEndPoint() );
}

// 
void handleSetNoMute() {
  resetTimer();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  isMute = false ;
  server.send(200, "text/plain", "Parameters received");
}

// 
void handleSetMute() {
  resetTimer();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  isMute = true ;
  server.send(200, "text/plain", "Parameters received");
}

// 
void handleReset() {
  resetTimer();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  buzzerAktif = false ;
  isVibrating      = false;
  waspadaReported  = false;
  dangerReported   = false;
  server.send(200, "text/plain", "Parameters received");
}

void setup_server(){
  server.on("/"           , handleRoot      );
  server.on("/get"        , handleGet       );
  server.on("/setnomute"  , handleSetNoMute );
  server.on("/setmute"    , handleSetMute   );
  server.on("/reset"      , handleReset     );
}