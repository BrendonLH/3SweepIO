#include <Arduino.h>

#define UWB_RX 20
#define UWB_TX 21

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, UWB_RX, UWB_TX);

  Serial.println("Listening...");
}

void loop() {
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}