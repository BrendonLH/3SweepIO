#include <esp_now.h>
#include <WiFi.h>
uint8_t robotMAC[] = {0x00, 0x70, 0x07, 0x82, 0xBC, 0x48};

typedef struct {
  int command;
} ControlPacket;

ControlPacket data;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  memcpy(peerInfo.peer_addr, robotMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);

  Serial.println("Controller ready.");
}

void loop() {

  // Forward 1 second
  data.command = 1;
  for (int i = 0; i < 20; i++) {
    esp_now_send(robotMAC, (uint8_t *)&data, sizeof(data));
    delay(50);
  }

  // Backward 1 second
  data.command = 2;
  for (int i = 0; i < 20; i++) {
    esp_now_send(robotMAC, (uint8_t *)&data, sizeof(data));
    delay(50);
  }
}
