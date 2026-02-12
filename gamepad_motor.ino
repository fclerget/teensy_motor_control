#include <Bluepad32.h>

// ===== MDDS30 PIN MAP =====
#define PIN_AN1 25   // Left motor PWM
#define PIN_IN1 26   // Left motor DIR
#define PIN_AN2 27   // Right motor PWM
#define PIN_IN2 33   // Right motor DIR

// ===== PWM CONFIG =====
#define PWM_FREQ 20000
#define PWM_BITS 8
#define PWM_CH_L 0
#define PWM_CH_R 1

// ===== CONTROL TUNING =====
#define DEADZONE 40
#define RAMP_STEP 6
#define LOOP_MS 5
#define PRINT_MS 50
#define STARTUP_STOP_MS 5000

GamepadPtr pad = nullptr;

int currentL = 0;
int currentR = 0;
int targetL = 0;
int targetR = 0;

bool motorsEnabled = false;
unsigned long bootTime = 0;

// ===== Utility =====
int clamp255(int v) {
  if (v > 255) return 255;
  if (v < -255) return -255;
  return v;
}

int axisToPWM(int a) {
  if (abs(a) < DEADZONE) return 0;
  return clamp255((long)a * 255L / 512L);
}

int ramp(int cur, int tgt) {
  if (cur < tgt - RAMP_STEP) return cur + RAMP_STEP;
  if (cur > tgt + RAMP_STEP) return cur - RAMP_STEP;
  return tgt;
}

void writeMDDS30(int left, int right) {
  left = clamp255(left);
  right = clamp255(right);

  digitalWrite(PIN_IN1, left < 0 ? HIGH : LOW);
  digitalWrite(PIN_IN2, right < 0 ? HIGH : LOW);

  ledcWrite(PWM_CH_L, abs(left));
  ledcWrite(PWM_CH_R, abs(right));
}

// ===== Bluepad callbacks =====
void onConnectedGamepad(GamepadPtr gp) {
  pad = gp;
  Serial.println("PS5 CONNECTED");
}

void onDisconnectedGamepad(GamepadPtr gp) {
  (void)gp;
  pad = nullptr;
  Serial.println("PS5 DISCONNECTED");
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_AN1, OUTPUT);
  pinMode(PIN_AN2, OUTPUT);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);

  ledcSetup(PWM_CH_L, PWM_FREQ, PWM_BITS);
  ledcAttachPin(PIN_AN1, PWM_CH_L);
  ledcWrite(PWM_CH_L, 0);

  ledcSetup(PWM_CH_R, PWM_FREQ, PWM_BITS);
  ledcAttachPin(PIN_AN2, PWM_CH_R);
  ledcWrite(PWM_CH_R, 0);

  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  bootTime = millis();
  motorsEnabled = false;

  Serial.println("Holding STOP for 5 seconds (MDDS30 safety)");
}

// ===== Loop =====
void loop() {
  BP32.update();

  // Safety startup delay
  if (!motorsEnabled) {
    writeMDDS30(0, 0);
    if (millis() - bootTime >= STARTUP_STOP_MS) {
      motorsEnabled = true;
      Serial.println("Motors ENABLED");
    }
    delay(LOOP_MS);
    return;
  }

  targetL = 0;
  targetR = 0;

  if (pad && pad->isConnected()) {
    targetL = -axisToPWM(pad->axisY());   // Left stick Y
    targetR = -axisToPWM(pad->axisRY());  // Right stick Y
  }

  currentL = ramp(currentL, targetL);
  currentR = ramp(currentR, targetR);

  writeMDDS30(currentL, currentR);

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= PRINT_MS) {
    Serial.print("L=");
    Serial.print(currentL);
    Serial.print("  R=");
    Serial.println(currentR);
    lastPrint = millis();
  }

  delay(LOOP_MS);
}
