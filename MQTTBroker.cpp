#include "MQTTBroker.h"

MQTTBroker::MQTTBroker(RemoteDebug* debug, const char* hostname, const char* mqtt_user, const char* mqtt_password, int max_retries) {
  WiFiClient espClient;
  _mqttClient = new PubSubClient(espClient);
  _debug = debug;
  _hostname = hostname;
  _mqtt_user = mqtt_user;
  _mqtt_password = mqtt_password;
  _max_retries = max_retries;
}

void MQTTBroker::SetupMQTTRemoteServer(const char* mqtt_host,int mqtt_port) { _mqttClient->setServer(mqtt_host, mqtt_port); }

bool MQTTBroker::IsConnected() { return _mqttClient->connected(); }

bool MQTTBroker::Connect() {

  if(_mqttClient->connect(_hostname, _mqtt_user, _mqtt_password)) {
    char topic[strlen(_hostname) + strlen("/status")];
    strcpy(topic, _hostname);
    strcat(topic, "/status");
    puts (topic);
    
    char msg[strlen(_hostname) + strlen("connected: ")];
    strcpy(msg, _hostname);
    strcat(msg, "connected: ");
    puts (msg);

    //send keep alive
    _mqttClient->publish(topic, msg);
    _debug->println("publish MqttClient connected");
    return true;
  }

  return false;
}

void MQTTBroker::Reconnect() {

  _number_retries = 0;
  while(!IsConnected())
  {
    _number_retries++;
    if(_number_retries > _max_retries && !Connect())
    {      
      _debug->println("Connection to MQTT Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
    
    delay(1000); // wait 1 second for reconnect
  }

}

void MQTTBroker::CheckConnection() {

  if(!IsConnected()) {
    Reconnect();
  }

  _mqttClient->loop();
}

bool MQTTBroker::SendMQTTMessage(const char* topic, char* payload) {
  
  if(!_mqttClient->connected()) {
    _debug->println("Can't send the message, because mqttClient is not connected");
    return false;
  }
    return _mqttClient->publish(topic, payload, false);
}