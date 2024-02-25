#ifndef Utils_h
#define Utils_h

#include "FileManager.h"
#include "config.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class Utils {
  public:
    static bool IsValidPort(int port);
    static bool StringIsNullOrEmpty(const char* str);
    static int GetChipId();
    static String IpAddress2String(const IPAddress& ipAddress);
    static void SaveBoardConfiguration(const char* configurationPath, FileManager* fileManager, const Config& config);
    static void ReadBoardConfiguration(const char* configurationPath, FileManager* fileManager, Config& config);
};

#endif
