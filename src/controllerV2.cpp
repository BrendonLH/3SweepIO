#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Joystick pins
#define JOY_X 2
#define JOY_Y 3
#define JOY_SW 4

unsigned long lastSendTime = 0;
int sendInterval = 50; // ms

uint8_t robotMAC[] = {0x14, 0x33, 0x5C, 0x03, 0x10, 0xAC};

bool steeringInverted = false;
bool lastButtonState = HIGH;

typedef struct
{
    int throttle; // -255 to 255
    int steering; // -255 to 255

} ControlPacket;

ControlPacket data;
esp_now_peer_info_t peerInfo;

// joystick calibration
int xCenterVal = 2790;
int yCenterVal = 2330;
int deadzone = 300; // more give around center

int lastThrottle = 999;
int lastSteering = 999;

int readAxis(int value, int center)
{
    int diff = value - center;

    if (abs(diff) < deadzone)
    {
        return 0;
    }

    if (diff > 0)
    {
        return map(diff, deadzone, 4095 - center, 0, 255);
    }
    else
    {
        return map(diff, -deadzone, -center, 0, -255);
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    pinMode(JOY_SW, INPUT_PULLUP);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    memcpy(peerInfo.peer_addr, robotMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    delay(2000);

    Serial.println("=== CONTROLLER BOOTED ===");
}

void loop()
{
    int x = analogRead(JOY_X);
    int y = analogRead(JOY_Y);

    bool buttonState = digitalRead(JOY_SW);

    if (lastButtonState == HIGH && buttonState == LOW)
    {
        steeringInverted = !steeringInverted;

        Serial.print("Steering mode: ");
        Serial.println(steeringInverted ? "INVERTED" : "NORMAL");
    }

    lastButtonState = buttonState;

    // X is forward/backward on your setup
    // Y is left/right steering on your setup
    int throttle = readAxis(x, xCenterVal);
    int steering = readAxis(y, yCenterVal);

    // Flip directions if needed
    throttle = -throttle;
    if (steeringInverted)
    {
        steering = -steering;
    }

    unsigned long now = millis();

    bool changed =
        throttle != lastThrottle ||
        steering != lastSteering;

    if (changed || now - lastSendTime > sendInterval)
    {
        data.throttle = throttle;
        data.steering = steering;

        esp_now_send(robotMAC, (uint8_t *)&data, sizeof(data));

        if (changed)
        {
            Serial.print("Throttle: ");
            Serial.print(throttle);
            Serial.print(" | Steering: ");
            Serial.print(steering);
            Serial.print(" | Mode: ");
            Serial.println(steeringInverted ? "INVERTED" : "NORMAL");
        }

        lastThrottle = throttle;
        lastSteering = steering;
        lastSendTime = now;
    }

    delay(20);
}