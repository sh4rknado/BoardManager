#ifndef FirmwareManager_h
#define FirmwareManager_h

#include "Utils.h"
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>

class FirmwareManager {
  private:
    RemoteDebug* _debug;

  public:
    FirmwareManager(RemoteDebug* debug);
    void SetupFirmware(const char* hostName, int port, bool auth, const char* password);
    void CheckFirmwareUpdate();
    void OnUpdateStart();
    void OnUpdateEnd();
    void OnUpdateProgress(unsigned int progress, unsigned int total);
    void OnUpdateError(ota_error_t error);
};

#endif