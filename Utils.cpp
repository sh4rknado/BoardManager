#include "Utils.h"

bool Utils::IsValidPort(int port) {
  return port > 0 && port < 65535;
}

bool Utils::StringIsNullOrEmpty(const char* str) {
  return str == nullptr || *str == '\0';
}

int Utils::GetChipId() {

#ifdef ESP8266
  return ESP.getChipId();
#endif

#ifdef ESP32
  uint64_t macAddress = ESP.getEfuseMac();
  uint64_t macAddressTrunc = macAddress << 40;
  return macAddressTrunc >> 40;
#endif
}

String Utils::IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}