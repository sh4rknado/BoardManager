#ifndef Config_h
#define Config_h

#include <Arduino.h>

struct Config {
  // General Section
  char* hostname;

  // Wifi Section
  char* wifi_name;
  char* wifi_password;
  
  // OTA SECTION
  char* password_ota;
  bool ota_auth;
  int ota_port;

  // MQTT section
  char* mqtt_user;
  char* mqtt_password;
  char* mqtt_host;
  int mqtt_port;
  int max_retries;

  // NTP Section
  char* ntp_server_1;
  char* ntp_server_2; 
  char* ntp_server_3;
  long timezone;
  byte daysavetime;
};

#endif