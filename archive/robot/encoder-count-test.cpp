#include <Arduino.h>

// encoder-count-test.cpp

// Motor A
#define RPWM_A 18
#define LPWM_A 19

// Motor B
#define RPWM_B 26
#define LPWM_B 25

// Encoders
#define LEFT_ENCODER 34
#define RIGHT_ENCODER 23

int chA_R = 0;
int chA_L = 1;
int chB_R = 2;
int chB_L = 3;

volatile long leftCount = 0;
volatile long rightCount = 0;

const int testSpeed = 180;
const unsigned long runTime = 10000; // 10 seconds

unsigned long startTime = 0;
bool testRunning = true;

void IRAM_ATTR countLeft()
{
    leftCount++;
}

void IRAM_ATTR countRight()
{
    rightCount++;
}

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

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("=== ENCODER COUNT TEST ===");

    ledcSetup(chA_R, 1000, 8);
    ledcSetup(chA_L, 1000, 8);
    ledcSetup(chB_R, 1000, 8);
    ledcSetup(chB_L, 1000, 8);

    ledcAttachPin(RPWM_A, chA_R);
    ledcAttachPin(LPWM_A, chA_L);
    ledcAttachPin(RPWM_B, chB_R);
    ledcAttachPin(LPWM_B, chB_L);

    pinMode(LEFT_ENCODER, INPUT);
    pinMode(RIGHT_ENCODER, INPUT);

    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER), countLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), countRight, RISING);

    stopMotors();

    Serial.println("Starting motors...");
    startTime = millis();

    setMotorA(165); // right motor
    setMotorB(180); // left motor
}

void loop()
{
    unsigned long now = millis();

    static unsigned long lastPrint = 0;

    if (testRunning && now - lastPrint >= 500)
    {
        lastPrint = now;

        Serial.print("Left: ");
        Serial.print(leftCount);
        Serial.print(" | Right: ");
        Serial.println(rightCount);
    }

    if (testRunning && now - startTime >= runTime)
    {
        stopMotors();
        testRunning = false;

        Serial.println("=== TEST COMPLETE ===");
        Serial.print("Final Left Count: ");
        Serial.println(leftCount);
        Serial.print("Final Right Count: ");
        Serial.println(rightCount);
        Serial.print("Difference: ");
        Serial.println(leftCount - rightCount);
    }
}