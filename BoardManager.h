#ifndef BoardManager_h
#define BoardManager_h

#include "Utils.h"
#include "config.h"
#include "FirmwareManager.h"
#include "FileManager.h"
#include "MQTTBroker.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <RemoteDebug.h>

class BoardManager {
  private:
    RemoteDebug* _debug;
    FirmwareManager* _firmware;
    MQTTBroker* _mqttBroker;
    FileManager* _fileManager;
    Config* _config;
    
  public:
    BoardManager();
    void Setup();
    void CheckFirmwareUpdate();
    void CheckWiFiConnection();
    bool MQTTConnect();
    void CheckMQTTConnection();
    void Log(String msg);
    void CheckDebug();
    bool SendMQTTMessage(const char* topic, char* payload);
  
  private:
    void SetupNTP(long timezone, byte daysavetime, const char* ntpServer1, const char* ntpServer2, const char* ntpServer3);
    void SetupWiFi(const char* ssid, const char* password);
    void SetupFileSystem();
    
};

#endif
