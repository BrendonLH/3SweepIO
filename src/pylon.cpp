#include <Arduino.h>

// ESP32-C3 + Reyax Lite122 UART Auto Test
// RED    VDD  -> 3.3V
// BLACK  GND  -> GND
// GREEN  TXD  -> GPIO20 RX
// YELLOW RXD  -> GPIO21 TX
// ORANGE NRST -> GPIO4

#define UWB_RX 20
#define UWB_TX 21
#define UWB_RESET 4

unsigned long lastSend = 0;

void resetUWB()
{
  digitalWrite(UWB_RESET, LOW);
  delay(100);
  digitalWrite(UWB_RESET, HIGH);
  delay(1000);
}

void setup()
{
  Serial.begin(115200);

  delay(3000);   // Give monitor time to connect

  Serial.println("BOOT");

  pinMode(UWB_RESET, OUTPUT);
  digitalWrite(UWB_RESET, HIGH);

  Serial1.begin(115200, SERIAL_8N1, UWB_RX, UWB_TX);

  resetUWB();

  Serial.println("=== Reyax Lite122 Terminal ===");
  Serial.println("Type AT commands and press Enter");
}

void loop()
{
  // PC -> Reyax
while (Serial.available())
{
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd.length() > 0)
  {
    Serial.print("You typed: ");
    Serial.println(cmd);

    Serial1.print(cmd);
    Serial1.print("\r\n");
  }
}

  // Reyax -> PC
  while (Serial1.available())
  {
    Serial.write(Serial1.read());
  }
}