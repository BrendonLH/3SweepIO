#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "secrets.h"

// Joystick pins
#define JOY_X 2
#define JOY_Y 3
#define JOY_SW 4

// Commands
#define CMD_STOP 0
#define CMD_LEFT 1
#define CMD_RIGHT 2
#define CMD_DOWN 3
#define CMD_UP 4
#define CMD_CLICK 5

unsigned long lastSendTime = 0;
int sendInterval = 50; // ms

uint8_t robotMAC[] = {0x14, 0x33, 0x5C, 0x03, 0x10, 0xAC};

typedef struct
{
    int command;
} ControlPacket;

ControlPacket data;
esp_now_peer_info_t peerInfo;

int lastCommand = -1;

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

    esp_now_add_peer(&peerInfo);

    delay(2000); // give monitor time to connect

    Serial.println("=== CONTROLLER BOOTED ===");
}

void loop()
{
    int x = analogRead(JOY_X);
    int y = analogRead(JOY_Y);

    int button = digitalRead(JOY_SW);
    bool pressed = (button == LOW); // active LOW

    int command = CMD_STOP;

    int xCenterVal = 2790;
    int yCenterVal = 2330;
    int deadzone = 200;

    bool xCenter = (x > xCenterVal - deadzone && x < xCenterVal + deadzone);
    bool yCenter = (y > yCenterVal - deadzone && y < yCenterVal + deadzone);

    // 🔥 CLICK HAS PRIORITY
    if (pressed)
    {
        command = CMD_CLICK;
    }
    else if (!yCenter)
    {
        if (y > yCenterVal + deadzone)
        {
            command = CMD_LEFT;
        }
        else
        {
            command = CMD_RIGHT;
        }
    }
    else if (!xCenter)
    {
        if (x > xCenterVal + deadzone)
        {
            command = CMD_DOWN;
        }
        else
        {
            command = CMD_UP;
        }
    }
    else
    {
        command = CMD_STOP;
    }

    // ✅ Only send when command changes
    unsigned long now = millis();

    if (command == CMD_STOP || command != lastCommand || (now - lastSendTime > sendInterval))
    {
        data.command = command;
        esp_now_send(robotMAC, (uint8_t *)&data, sizeof(data));

        // only print when command changes (keeps console clean)
        if (command != lastCommand)
        {
            Serial.print("CMD: ");
            Serial.println(command);
        }

        lastCommand = command;
        lastSendTime = now;
    }

    delay(30);
}