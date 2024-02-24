#ifndef BoardManager_h
#define BoardManager_h

#include "Utils.h"
#include "FirmwareManager.h"
#include "FileManager.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <RemoteDebug.h>

class BoardManager {
  private:
    FirmwareManager* _firmware;
    RemoteDebug* _debug;
    FileManager* _fileManager;

  public:
    BoardManager(const char* hostName);
    void SetupNTP(long timezone, byte daysavetime, const char* ntpServer1, const char* ntpServer2, const char* ntpServer3);
    void SetupFirmware(int port, bool auth, const char* password);
    void SetupWiFi(const char* ssid, const char* password);
    void SetupFileSystem();
    void CheckFirmwareUpdate();
    void CheckWiFiConnection();
    void Log(String msg);
    void CheckDebug();
};

#endif
