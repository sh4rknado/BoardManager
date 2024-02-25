#ifndef MQTTBroker_h
#define MQTTBroker_h

#include <RemoteDebug.h>
#include <PubSubClient.h>
#include "config.h"

class MQTTBroker {
  private:
    RemoteDebug* _debug;
    PubSubClient* _mqttClient;
    Config* _config;

  public:
    MQTTBroker(RemoteDebug* debug);
    void SetupMQTTRemoteServer(Config* config);
    bool IsConnected();
    bool Connect();
    void Reconnect();
    void CheckConnection();
    bool SendMQTTMessage(const char* topic, char* payload);

};

#endif