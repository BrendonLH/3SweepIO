#include <Arduino.h>

// Motor A
#define RPWM_A 19
#define LPWM_A 21

// Motor B
#define RPWM_B 25
#define LPWM_B 26

int chA_R = 0;
int chA_L = 1;
int chB_R = 2;
int chB_L = 3;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("BOOTED");

  // Setup PWM
  ledcSetup(chA_R, 20000, 8);
  ledcSetup(chA_L, 20000, 8);
  ledcSetup(chB_R, 20000, 8);
  ledcSetup(chB_L, 20000, 8);

  ledcAttachPin(RPWM_A, chA_R);
  ledcAttachPin(LPWM_A, chA_L);
  ledcAttachPin(RPWM_B, chB_R);
  ledcAttachPin(LPWM_B, chB_L);

  Serial.println("PWM READY");

  // Forward both motors
  ledcWrite(chA_R, 0);
  ledcWrite(chA_L, 255);

  ledcWrite(chB_R, 0);
  ledcWrite(chB_L, 255);

  Serial.println("BOTH MOTORS FORWARD");
}

void loop()
{
  delay(1000);
}