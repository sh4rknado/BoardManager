#ifndef MQTTBroker_h
#define MQTTBroker_h

#include <RemoteDebug.h>
#include <PubSubClient.h>

class MQTTBroker {
  private:
    RemoteDebug* _debug;
    PubSubClient* _mqttClient;
    const char* _hostname; 
    const char* _mqtt_user;
    const char* _mqtt_password;
    int _number_retries;
    int _max_retries;

  public:
    MQTTBroker(RemoteDebug* debug, const char* hostname, const char* mqtt_user, const char* mqtt_password, int max_retries);
    void SetupMQTTRemoteServer(const char* mqtt_host,int mqtt_port);
    void CheckMQTTClient(long cycleTime);
    bool IsConnected();
    bool Connect();
    void Reconnect();
    void CheckConnection();
    bool SendMQTTMessage(const char* topic, char* payload);

};

#endif