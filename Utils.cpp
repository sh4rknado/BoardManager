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

void Utils::ReadBoardConfiguration(const char* configurationPath, FileManager* fileManager, Config& config) {
  JsonDocument doc; // Allocate a temporary JsonDocument

  if(fileManager->ReadJson(configurationPath, doc))
  {
    //general section
    strlcpy(config.hostname, doc["hostname"] | "esp8266", sizeof(config.hostname));
    
    //wifi section
    strlcpy(config.wifi_name, doc["wifi_name"] | "", sizeof(config.wifi_name));
    strlcpy(config.wifi_password, doc["wifi_password"] | "", sizeof(config.wifi_password));

    // OTA Section
    strlcpy(config.password_ota, doc["password_ota"] | "admin", sizeof(config.password_ota));
    config.ota_port = doc["ota_port"] | 8266;
    config.ota_auth = doc["ota_auth"] | true;
    
    // MQTT Section
    strlcpy(config.mqtt_user, doc["mqtt_user"] | "esp8266", sizeof(config.mqtt_user));
    strlcpy(config.mqtt_password, doc["mqtt_password"] | "admin", sizeof(config.mqtt_password));
    strlcpy(config.mqtt_host, doc["mqtt_host"] | "", sizeof(config.mqtt_host));
    config.mqtt_port = doc["mqtt_port"] | 8080;
    config.max_retries = doc["max_retries"] | 5;

    // NTP Section
    strlcpy(config.ntp_server_1, doc["ntp_server_1"] | "", sizeof(config.ntp_server_1));
    strlcpy(config.ntp_server_2, doc["ntp_server_2"] | "", sizeof(config.ntp_server_2));
    strlcpy(config.ntp_server_3, doc["ntp_server_3"] | "", sizeof(config.ntp_server_3));
    config.timezone = doc["timezone"] | 0;
    config.daysavetime = doc["daysavetime"] | 1;
  }
  
}

void Utils::SaveBoardConfiguration(const char* configurationPath, FileManager* fileManager, const Config& config) {
  JsonDocument doc;

  // General Section
  doc["hostname"] = config.hostname;
  
  // Wifi Section
  doc["wifi_name"] = config.wifi_name;
  doc["wifi_password"] = config.wifi_password;

  // OTA SECTION
  doc["password_ota"] = config.password_ota;
  doc["ota_auth"] = config.ota_auth;
  doc["ota_port"] = config.ota_port;

  // MQTT section
  doc["mqtt_user"] = config.mqtt_user;
  doc["mqtt_password"] = config.mqtt_password;
  doc["mqtt_host"] = config.mqtt_host;
  doc["mqtt_port"] = config.mqtt_port;
  doc["max_retries"] = config.max_retries;

  // NTP Section
  doc["ntp_server_1"] = config.ntp_server_1;
  doc["ntp_server_2"] = config.ntp_server_2;
  doc["ntp_server_3"] = config.ntp_server_3;
  doc["timezone"] = config.timezone;
  doc["daysavetime"] = config.daysavetime;

  fileManager->WriteJson(configurationPath, doc);
  
}