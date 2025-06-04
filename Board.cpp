#include "Board.h"

Board::Board() {
  _debug = new RemoteDebug();  
  _fileManager = new FileManager(_debug);
  _firmware = new FirmwareManager(_debug, _fileManager);
}

Config Board::GetConfiguration() { return _config; }

// #################################################### < SETUP REGION > ###########################################################

void Board::Setup() {
  // mount file system
  _fileManager->Mount(); 
  
  // read config
  ReadBoardConfiguration("/config.json", _fileManager, _config);
  
  // setup wifi configuration
  WiFi.mode(WIFI_STA);
  ConnectToWifi();

  // setup telnet debug
  _debug->begin(_config.hostname, RemoteDebug::ANY);

  // setup firmware OTA
  _firmware->SetupFirmware(_config.hostname, _config.ota_port, _config.ota_auth, _config.password_ota);
}

void Board::SetupNTP() {
  configTime(3600 * _config.timezone, _config.daysavetime * 3600,  _config.ntp_server_1,  _config.ntp_server_2,  _config.ntp_server_3);
  struct tm tmstruct;
  delay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  _debug->printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
}

// #################################################### < Handle REGION > ###########################################################

void Board::Handle() { 
  _firmware->CheckFirmwareUpdate();  //handle OTA
  _debug->handle();  //handle debug
}

// #################################################### < CONNECTIONS > ###########################################################

void Board::ConnectToWifi() {
  WiFi.begin(_config.wifi_name, _config.wifi_password);
  
  Serial.println("\n");
  Serial.println("Connecting to WiFi");

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.printf(".");
	}

	Serial.println("\nConnected to WiFi");
	Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
}

// #################################################### < CONFIGURATION > ###########################################################

void Board::ReadBoardConfiguration(const char* configurationPath, FileManager* fileManager, Config& config) {
  JsonDocument doc; // Allocate a temporary JsonDocument
  
  fileManager->DeleteFile(configurationPath);

  if(!fileManager->Exists(configurationPath)) {
    ApplyDefaultConfiguration(configurationPath, fileManager, config);
  }
  else if(fileManager->ReadJson(configurationPath, doc)) {
    
    Serial.println("Configuration found, read configuration...");

    //general section
    config.hostname = doc["hostname"] | "esp8266";
    
    //wifi section
    config.wifi_name = doc["wifi_name"] | "wifi";
    config.wifi_password = doc["wifi_password"] | "pass";

    // OTA Section
    config.password_ota = doc["password_ota"] | "admin";
    config.ota_port = doc["ota_port"] | 8266;
    config.ota_auth = doc["ota_auth"] | true;
    
    // // MQTT Section
    config.mqtt_user = doc["mqtt_user"] | "admin";
    config.mqtt_password = doc["mqtt_password"] | "admin";
    config.mqtt_host = doc["mqtt_host"] | "192.168.1.200";
    config.mqtt_port = doc["mqtt_port"] | 8080;
    config.max_retries = doc["max_retries"] | 5;

    // NTP Section
    config.ntp_server_1 = doc["ntp_server_1"] | "1.be.pool.ntp.org";
    config.ntp_server_2 = doc["ntp_server_2"] | "2.be.pool.ntp.org";
    config.ntp_server_3 = doc["ntp_server_3"] | "3.be.pool.ntp.org";
    config.timezone = doc["timezone"] | 0;
    config.daysavetime = doc["daysavetime"] | 1;

    // debug json serialize
    // serializeJson(doc, Serial);
  }
}

void Board::ApplyDefaultConfiguration(const char* configurationPath, FileManager* fileManager, Config& config) {
    
    Serial.println("Configuration not found, default configuration will be used");

    //general section
    config.hostname = "esp8266";

    //wifi section
    config.wifi_name = "WiFi-2.4-DC03";
    config.wifi_password = "AC4635D555";
    
    // OTA Section
    config.password_ota = "admin";
    config.ota_port = 8266;
    config.ota_auth = true;
    
    // MQTT Section
    config.mqtt_user = "admin";
    config.mqtt_password = "admin";
    config.mqtt_host = "192.168.1.200";
    config.mqtt_port = 8080;
    config.max_retries = 5;

    // NTP Section
    config.ntp_server_1 = "1.be.pool.ntp.org";
    config.ntp_server_2 = "2.be.pool.ntp.org";
    config.ntp_server_3 = "3.be.pool.ntp.org";
    config.timezone = 0;
    config.daysavetime = 1;

    SaveBoardConfiguration(configurationPath, fileManager, config);
}

void Board::SaveBoardConfiguration(const char* configurationPath, FileManager* fileManager, const Config& config) {
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

  Serial.println("Write default configuration...");
  fileManager->WriteJson(configurationPath, doc);
  Serial.println("save configuration completed");
}


