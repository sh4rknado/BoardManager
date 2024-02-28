#ifndef Board_h
#define Board_h

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <RemoteDebug.h>
#include "FirmwareManager.h"
#include "FileManager.h"
#include "config.h"

class Board {
  private:
    RemoteDebug* _debug;
    FirmwareManager* _firmware;
    FileManager* _fileManager;
    Config _config;

  public:
    Board();
    Config GetConfiguration();
    void Setup();
    void SetupNTP();
    void Handle();
    void ConnectToWifi();
    void SaveBoardConfiguration(const char* configurationPath, FileManager* fileManager, const Config& config);
    void ApplyDefaultConfiguration(const char* configurationPath, FileManager* fileManager, Config& config);
    void ReadBoardConfiguration(const char* configurationPath, FileManager* fileManager, Config& config);

};

#endif


