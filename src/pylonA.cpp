#include <Arduino.h>

// Pylon A = Anchor
// RED    VDD  -> 3.3V
// BLACK  GND  -> GND
// GREEN  TXD  -> GPIO20 RX
// YELLOW RXD  -> GPIO21 TX
// ORANGE NRST -> GPIO4

#define UWB_RX 20
#define UWB_TX 21
#define UWB_RESET 4

const unsigned long RANGE_INTERVAL = 2000;
const int SAMPLE_SIZE = 5;

unsigned long lastRangeCheck = 0;

int readings[SAMPLE_SIZE];
int readingIndex = 0;
int readingCount = 0;

void resetUWB()
{
  digitalWrite(UWB_RESET, LOW);
  delay(100);
  digitalWrite(UWB_RESET, HIGH);
  delay(3000);
}

void addReading(int cm)
{
  readings[readingIndex] = cm;
  readingIndex = (readingIndex + 1) % SAMPLE_SIZE;

  if (readingCount < SAMPLE_SIZE)
  {
    readingCount++;
  }
}

float getAverageCm()
{
  if (readingCount == 0)
  {
    return 0;
  }

  long total = 0;

  for (int i = 0; i < readingCount; i++)
  {
    total += readings[i];
  }

  return (float)total / readingCount;
}

void printFeetInches(float cm)
{
  float totalInches = cm / 2.54;
  int feet = totalInches / 12;
  int inches = round(totalInches - (feet * 12));

  if (inches == 12)
  {
    feet++;
    inches = 0;
  }

  Serial.print(feet);
  Serial.print("' ");
  Serial.print(inches);
  Serial.println("\"");
}

void parseReyaxLine(String line)
{
  line.trim();

  if (line.length() == 0)
  {
    return;
  }

  Serial.print("RAW: ");
  Serial.println(line);

  int cmIndex = line.indexOf(" cm");

  if (cmIndex == -1)
  {
    return;
  }

  int lastComma = line.lastIndexOf(',', cmIndex);

  if (lastComma == -1)
  {
    return;
  }

  String cmText = line.substring(lastComma + 1, cmIndex);
  cmText.trim();

  int cm = cmText.toInt();

  if (cm <= 0)
  {
    return;
  }

  addReading(cm);

  float avgCm = getAverageCm();

  Serial.print("Current: ");
  Serial.print(cm);
  Serial.println(" cm");

  Serial.print("Average: ");
  Serial.print(avgCm);
  Serial.print(" cm / ");

  printFeetInches(avgCm);
}

void setup()
{
  Serial.begin(115200);
  delay(3000);

  Serial.println("BOOTING PYLON A...");

  pinMode(UWB_RESET, OUTPUT);
  digitalWrite(UWB_RESET, HIGH);

  Serial1.begin(115200, SERIAL_8N1, UWB_RX, UWB_TX);

  resetUWB();

  Serial.println("=== Pylon A Auto Range ===");
  Serial.println("Target Tag: T001");
}

void loop()
{
  if (millis() - lastRangeCheck >= RANGE_INTERVAL)
  {
    Serial.println();
    Serial.println("Checking distance...");
    Serial1.print("AT+ANCHOR_SEND=T001,5,HELLO\r\n");

    lastRangeCheck = millis();
  }

  while (Serial1.available())
  {
    String line = Serial1.readStringUntil('\n');
    parseReyaxLine(line);
  }
}