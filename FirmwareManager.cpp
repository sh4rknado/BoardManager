#include "FirmwareManager.h"

FirmwareManager::FirmwareManager(RemoteDebug* debug) {
  _debug = debug;
}

void FirmwareManager::SetupFirmware(const char* hostName, int port, bool auth, const char* password) {
  
  // Port defaults to 8266
  if (Utils::IsValidPort(port)) {
    ArduinoOTA.setPort(port);
  } else {
    ArduinoOTA.setPort(8266);
  }

  // Hostname defaults to esp8266-[ChipID]
  if (Utils::StringIsNullOrEmpty(hostName)) {
    ArduinoOTA.setHostname(hostName);
  } else {
    ArduinoOTA.setHostname("esp8266" + Utils::GetChipId());
  }

  if (auth) {
    ArduinoOTA.setPassword(password);
  }

  ArduinoOTA.onStart([this]() { this->OnUpdateStart(); });
  ArduinoOTA.onEnd([this]() { this->OnUpdateEnd(); });
  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) { this->OnUpdateProgress(progress, total); });
  ArduinoOTA.onError([this](ota_error_t error) { this->OnUpdateError(error); });
  ArduinoOTA.begin();
}

void FirmwareManager::CheckFirmwareUpdate() {
  ArduinoOTA.handle();
}

void FirmwareManager::OnUpdateStart() {
  // NOTE: if updating FS this would be the place to unmount FS using FS.end()
  if (ArduinoOTA.getCommand() == U_FLASH) {
    _debug->println("Start updating sketch");
  } else {  // U_FS
    _debug->println("Start updating U_FS");
  }
}

void FirmwareManager::OnUpdateEnd() {
 _debug->println("\nEnd");
}

void FirmwareManager::OnUpdateProgress(unsigned int progress, unsigned int total) {
  float percentage = (progress / (total / 100));
  _debug->println("Progress: " + String(percentage) + " %"); 
}

void FirmwareManager::OnUpdateError(ota_error_t error) {
 switch (error) {
      case OTA_AUTH_ERROR:
      _debug->println("Auth Failed");
        break;
      case OTA_BEGIN_ERROR:
       _debug->println("Begin Failed");
        break;
      case OTA_CONNECT_ERROR:
       _debug->println("connect Failed");
        break;
      case OTA_RECEIVE_ERROR:
       _debug->println("Receive Failed");
        break;
      case OTA_END_ERROR:
       _debug->println("End Failed");
        break;
      default:
       _debug->println("Unknow error:" + String(error));
        break;
    }
}
