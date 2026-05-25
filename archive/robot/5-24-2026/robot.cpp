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

typedef struct
{
  int throttle; // -255 to 255
  int steering; // -255 to 255
  bool click;
} ControlPacket;

ControlPacket incomingData;

unsigned long lastPacketTime = 0;
const unsigned long timeout = 300;

void stopMotors()
{
  ledcWrite(chA_R, 0);
  ledcWrite(chA_L, 0);
  ledcWrite(chB_R, 0);
  ledcWrite(chB_L, 0);
}

void setMotorA(int power)
{
  power = constrain(power, -255, 255);

  if (power > 0)
  {
    ledcWrite(chA_R, 0);
    ledcWrite(chA_L, power);
  }
  else if (power < 0)
  {
    ledcWrite(chA_R, -power);
    ledcWrite(chA_L, 0);
  }
  else
  {
    ledcWrite(chA_R, 0);
    ledcWrite(chA_L, 0);
  }
}

void setMotorB(int power)
{
  power = constrain(power, -255, 255);

  if (power > 0)
  {
    ledcWrite(chB_R, 0);
    ledcWrite(chB_L, power);
  }
  else if (power < 0)
  {
    ledcWrite(chB_R, -power);
    ledcWrite(chB_L, 0);
  }
  else
  {
    ledcWrite(chB_R, 0);
    ledcWrite(chB_L, 0);
  }
}

void driveMixed(int throttle, int steering)
{
  int leftPower = throttle + steering;
  int rightPower = throttle - steering;

  leftPower = constrain(leftPower, -255, 255);
  rightPower = constrain(rightPower, -255, 255);

  setMotorA(leftPower);
  setMotorB(rightPower);
}

void onReceive(const uint8_t *mac, const uint8_t *incoming, int len)
{
  memcpy(&incomingData, incoming, sizeof(incomingData));
  lastPacketTime = millis();

  int throttle = incomingData.throttle;
  int steering = incomingData.steering;

  Serial.print("Throttle: ");
  Serial.print(throttle);
  Serial.print(" | Steering: ");
  Serial.print(steering);
  Serial.print(" | Click: ");
  Serial.println(incomingData.click);

  driveMixed(throttle, steering);
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

  if (now - lastPacketTime < timeout)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, (now % 1000 < 500));
    stopMotors();
  }
}