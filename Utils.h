#ifndef Utils_h
#define Utils_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

class Utils {
  public:
    static bool IsValidPort(int port);
    static bool StringIsNullOrEmpty(const char* str);
    static int GetChipId();
    static String IpAddress2String(const IPAddress& ipAddress);
};

#endif
