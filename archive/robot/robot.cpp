#include <esp_now.h>
#include <WiFi.h>

// Motor A
#define RPWM_A 18
#define LPWM_A 19

// Motor B
#define RPWM_B 26
#define LPWM_B 25
#define LED 2

int chA_R = 0;
int chA_L = 1;
int chB_R = 2;
int chB_L = 3;

int speedVal = 255;
int turnFast = 255;
int turnSlow = 120; // smoother curve

int lastCmd = 0;

typedef struct
{
  int command;
} ControlPacket;

ControlPacket incomingData;

unsigned long lastPacketTime = 0;
const unsigned long timeout = 300; // ms before we consider disconnected

void driveForward()
{
  ledcWrite(chA_R, 0);
  ledcWrite(chA_L, speedVal);

  ledcWrite(chB_R, 0);
  ledcWrite(chB_L, speedVal);
}

void driveBackward()
{
  ledcWrite(chA_R, speedVal);
  ledcWrite(chA_L, 0);

  ledcWrite(chB_R, speedVal);
  ledcWrite(chB_L, 0);
}
void turnLeft()
{
  // Left motor stop
  ledcWrite(chA_R, 0);
  ledcWrite(chA_L, speedVal);

  // Right motor forward (FLIPPED)
  ledcWrite(chB_R, 0);
  ledcWrite(chB_L, 0);
}
void turnRight()
{
  // Left motor FORWARD (same as driveForward)
  ledcWrite(chA_R, 0);
  ledcWrite(chA_L, 0);

  // Right motor STOP
  ledcWrite(chB_R, 0);
  ledcWrite(chB_L, speedVal);
}

void stopMotors()
{
  ledcWrite(chA_R, 0);
  ledcWrite(chA_L, 0);
  ledcWrite(chB_R, 0);
  ledcWrite(chB_L, 0);
}

void onReceive(const uint8_t *mac, const uint8_t *incoming, int len)
{
  memcpy(&incomingData, incoming, sizeof(incomingData));
  lastPacketTime = millis();

  int cmd = incomingData.command;

  Serial.print("Received CMD: ");
  Serial.println(cmd);

  if (cmd != lastCmd)
  {
    stopMotors();
    delay(10);
  }

  lastCmd = cmd;
  switch (cmd)
  {
  case 1:
    turnLeft();
    break;

  case 2:
    turnRight();
    break;

  case 3:
    driveBackward();
    break;

  case 4:
    driveForward();
    break;

  default:
    stopMotors();
    break;
  }
}
void setup()
{
  Serial.begin(115200);
  lastPacketTime = millis();
  ledcSetup(chA_R, 1000, 8);
  ledcSetup(chA_L, 1000, 8);
  ledcSetup(chB_R, 1000, 8);
  ledcSetup(chB_L, 1000, 8);

  ledcAttachPin(RPWM_A, chA_R);
  ledcAttachPin(LPWM_A, chA_L);
  ledcAttachPin(RPWM_B, chB_R);
  ledcAttachPin(LPWM_B, chB_L);
  pinMode(LED, OUTPUT);
  stopMotors();
  digitalWrite(LED, LOW);

  WiFi.mode(WIFI_STA);
  Serial.print("Robot MAC: ");
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Robot ready.");
}

void loop()
{

  unsigned long now = millis();

  // Heartbeat LED
  if (now - lastPacketTime < timeout)
  {
    digitalWrite(LED, HIGH); // Connected
  }
  else
  {
    digitalWrite(LED, (now % 1000 < 500)); // Slow blink if disconnected
    stopMotors();                          // Safety stop
  }
}