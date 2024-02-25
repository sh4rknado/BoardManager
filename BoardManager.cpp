//BoardManager.cpp

#include "BoardManager.h"

BoardManager::BoardManager() {
  _debug = new RemoteDebug();  
  _fileManager = new FileManager(_debug);
  _firmware = new FirmwareManager(_debug, _fileManager);
  _mqttBroker = new MQTTBroker(_debug);  
}

// #################################################### < SETUP REGION > ###########################################################

void BoardManager::Setup() {
  // mount file system
  _fileManager->Mount(); 

  // read config
  Utils::ReadBoardConfiguration("/config.json", _fileManager, *_config);
  
  // setup telnet debug
  _debug->begin(_config->hostname);

  // setup firmware OTA
  _firmware->SetupFirmware(_config->hostname, _config->ota_port, _config->ota_auth, _config->password_ota);
  
  // setup wifi configuration
  SetupWiFi(_config->wifi_name, _config->wifi_password);
  
  // setup NTP configuration (used by file system)
  SetupNTP(_config->timezone, _config->daysavetime, _config->ntp_server_1, _config->ntp_server_2, _config->ntp_server_3);

  // setup MQTT server
  _mqttBroker->SetupMQTTRemoteServer(_config);
}

void BoardManager::SetupNTP(long timezone, byte daysavetime, const char* ntpServer1, const char* ntpServer2, const char* ntpServer3) {
  configTime(3600 * timezone, daysavetime * 3600, ntpServer1, ntpServer2, ntpServer3);
  struct tm tmstruct;
  delay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  _debug->printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
}

void BoardManager::SetupWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  CheckWiFiConnection();

  _debug->println("Ready");
  _debug->println("IP address: ");
  _debug->println(Utils::Utils::IpAddress2String(WiFi.localIP()));
}

// #################################################### < CHECK REGION > ###########################################################

void BoardManager::CheckWiFiConnection() {

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    _debug->println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

}

void BoardManager::CheckFirmwareUpdate() { _firmware->CheckFirmwareUpdate(); }

void BoardManager::CheckDebug() { _debug->handle(); }

void BoardManager::CheckMQTTConnection() { _mqttBroker->CheckConnection(); }

// #################################################### < MQTT connection > ###########################################################

bool BoardManager::MQTTConnect() { return _mqttBroker->Connect(); }

bool BoardManager::SendMQTTMessage(const char* topic, char* payload) { return _mqttBroker->SendMQTTMessage(topic, payload); }

