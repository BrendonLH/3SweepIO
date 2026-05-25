#include <esp_now.h>
#include <WiFi.h>

// Motor A = Right Motor
#define RPWM_A 18
#define LPWM_A 19

// Motor B = Left Motor
#define RPWM_B 26
#define LPWM_B 25

#define LED 2

// Encoders
#define LEFT_ENCODER 34
#define RIGHT_ENCODER 23

int chA_R = 0;
int chA_L = 1;
int chB_R = 2;
int chB_L = 3;

const int MOTOR_A_OFFSET = 15;

// Auto correction settings
const int STRAIGHT_THROTTLE_MIN = 80;
const int STEERING_DEADZONE = 25;
const int CORRECTION_STEP = 1;
const int MAX_CORRECTION = 15;
const unsigned long CORRECTION_INTERVAL = 250;

volatile long leftTicks = 0;
volatile long rightTicks = 0;

int autoCorrection = 0;
unsigned long lastCorrectionTime = 0;

typedef struct
{
    int throttle;
    int steering;
} ControlPacket;

ControlPacket incomingData;

unsigned long lastPacketTime = 0;
const unsigned long timeout = 300;

void IRAM_ATTR countLeft()
{
    leftTicks++;
}

void IRAM_ATTR countRight()
{
    rightTicks++;
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

void updateAutoCorrection(int throttle, int steering)
{
    unsigned long now = millis();

    if (now - lastCorrectionTime < CORRECTION_INTERVAL)
    {
        return;
    }

    lastCorrectionTime = now;

    bool drivingStraight = abs(throttle) > STRAIGHT_THROTTLE_MIN && abs(steering) < STEERING_DEADZONE;

    if (!drivingStraight)
    {
        autoCorrection = 0;
        leftTicks = 0;
        rightTicks = 0;
        return;
    }

    noInterrupts();
    long left = leftTicks;
    long right = rightTicks;
    leftTicks = 0;
    rightTicks = 0;
    interrupts();

    // Safety check:
    // If either encoder is not producing ticks while driving straight,
    // do not apply auto correction.
    if ((left == 0 || right == 0) && abs(throttle) > 150)
    {
        autoCorrection = 0;
        Serial.println("Encoder fault detected. Auto correction disabled.");
        return;
    }

    int error = right - left;

    if (error > 1)
    {
        autoCorrection += CORRECTION_STEP; // right faster, slow right
    }
    else if (error < -1)
    {
        autoCorrection -= CORRECTION_STEP; // left faster, slow left
    }

    autoCorrection = constrain(autoCorrection, -MAX_CORRECTION, MAX_CORRECTION);

    Serial.print("L: ");
    Serial.print(left);
    Serial.print(" | R: ");
    Serial.print(right);
    Serial.print(" | Error: ");
    Serial.print(error);
    Serial.print(" | Correction: ");
    Serial.println(autoCorrection);
}

void driveMixed(int throttle, int steering)
{
    updateAutoCorrection(throttle, steering);

    int rightPower = throttle - steering;
    int leftPower = throttle + steering;

    rightPower = constrain(rightPower, -255, 255);
    leftPower = constrain(leftPower, -255, 255);

    int correctionDirection = throttle >= 0 ? 1 : -1;

    int correctedRight = rightPower - MOTOR_A_OFFSET - (autoCorrection * correctionDirection);
    int correctedLeft = leftPower + (autoCorrection * correctionDirection);

    setMotorA(constrain(correctedRight, -255, 255)); // Motor A = right
    setMotorB(constrain(correctedLeft, -255, 255));  // Motor B = left
}

void onReceive(const uint8_t *mac, const uint8_t *incoming, int len)
{
    memcpy(&incomingData, incoming, sizeof(incomingData));
    lastPacketTime = millis();

    driveMixed(incomingData.throttle, incomingData.steering);
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

    pinMode(LEFT_ENCODER, INPUT);
    pinMode(RIGHT_ENCODER, INPUT);

    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER), countLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), countRight, RISING);

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

    Serial.println("Robot ready with encoder correction.");
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
        autoCorrection = 0;
    }
}