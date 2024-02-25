#include "MQTTBroker.h"

MQTTBroker::MQTTBroker(RemoteDebug* debug) {
  WiFiClient espClient;
  _mqttClient = new PubSubClient(espClient);
  _debug = debug;
}

void MQTTBroker::SetupMQTTRemoteServer(Config* config) 
{ 
  _config = config;
  _mqttClient->setServer(_config->mqtt_host, _config->mqtt_port);
  Connect(); 
}

bool MQTTBroker::IsConnected() { return _mqttClient->connected(); }

bool MQTTBroker::Connect() {
    
  if(_mqttClient->connect(_config->hostname, _config->mqtt_user, _config->mqtt_password)) {
    char topic[strlen(_config->hostname) + strlen("/status")];
    strcpy(topic, _config->hostname);
    strcat(topic, "/status");
    puts (topic);
    
    char msg[strlen(_config->hostname) + strlen("connected: ")];
    strcpy(msg, _config->hostname);
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

  int number_retries = 0;
  while(!IsConnected())
  {
    number_retries++;
    if(number_retries > _config->max_retries && !Connect())
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