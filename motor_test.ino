#include "CytronMotorDriver.h"

// ---- Pin assignments for Arduino Nano ----
#define PWM_PIN 3     // PWM-capable pin
#define DIR_PIN 4     // Direction pin
#define LED_PIN 13    // Built-in LED (Nano onboard)

CytronMD motor1(PWM_DIR, PWM_PIN, DIR_PIN);

int speedVal = 0;
unsigned long lastToggle = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println("SmartDriveDuo-30 Motor + LED Speed Indicator");
  Serial.println("Enter speed between -255 and 255:");

  // --- Safe startup state ---
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(PWM_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  motor1.setSpeed(0);
}

void loop() {
  // --- Serial input ---
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    speedVal = input.toInt();
    speedVal = constrain(speedVal, -255, 255);

    motor1.setSpeed(speedVal);
    Serial.print("Motor speed set to: ");
    Serial.println(speedVal);
  }

  // --- LED blink frequency scales with |speedVal| ---
  int blinkRate = abs(speedVal);  // Hz
  if (blinkRate == 0) {
    digitalWrite(LED_PIN, LOW);
  } else {
    unsigned long periodMicros = 1000000UL / blinkRate / 2; // half-period in microseconds
    if (micros() - lastToggle >= periodMicros) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      lastToggle = micros();
    }
  }
}
