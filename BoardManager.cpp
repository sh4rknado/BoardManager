//BoardManager.cpp

#include "BoardManager.h"

BoardManager::BoardManager(const char* hostName) {
  _debug = new RemoteDebug();
  _debug->begin(hostName);
  
  _firmware = new FirmwareManager(_debug);
  _fileManager = new FileManager(_debug);
}

void BoardManager::SetupNTP(long timezone, byte daysavetime, const char* ntpServer1, const char* ntpServer2, const char* ntpServer3) {
  configTime(3600 * timezone, daysavetime * 3600, ntpServer1, ntpServer2, ntpServer3);
  struct tm tmstruct;
  delay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  _debug->printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
}

void BoardManager::SetupFirmware(const char *hostName, int port, bool auth, const char *password) {
  _firmware->SetupFirmware(hostName, port, auth, password);
}

void BoardManager::SetupFileSystem() {
  _fileManager->Mount();
}

void BoardManager::SetupWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    _debug->println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  _debug->println("Ready");
  _debug->println("IP address: ");
  _debug->println(Utils::Utils::IpAddress2String(WiFi.localIP()));
}

void BoardManager::CheckFirmwareUpdate() {
  _firmware->CheckFirmwareUpdate();
}

void BoardManager::CheckDebug(){
  _debug->handle();
}
