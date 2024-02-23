#include <BoardManager.h>

const char* hostName = "ESP8266";
const char* ssid = "WIFI_HOSTNAME";
const char* password = "PASSWORD";

BoardManager* board;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  board = new BoardManager(hostName); // define BoardManager with hostname
  board->SetupFirmware(hostName, 8266, true, "SECURE_PASSWORD_OTA"); // setup OTA Firmware
  board->SetupWiFi(ssid, password);
  board->SetupNTP(0, 1, "be.pool.ntp.org", "0.be.pool.ntp.org", "1.be.pool.ntp.org");
  board->SetupFileSystem(); // mount the LittleFS
}

void loop() {
  board->CheckFirmwareUpdate(); // handle for OTA update
  board->CheckDebug(); // handle for debug message
}
