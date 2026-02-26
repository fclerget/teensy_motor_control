// Teensy 4.1 wheel command receiver
// Receives: l_dir,l_pwm,r_dir,r_pwm\n

#include <Arduino.h>

static const uint32_t SERIAL_BAUD = 115200;

// ---- Pin assignments (Teensy 4.1) ----
//LEFT
#define PWM_PIN 15     // PWM-capable pin 
#define DIR_PIN 14     // Direction pin

//RIGHT
#define PWM_PIN2 7     // PWM-capable pin
#define DIR_PIN2 8     // Direction pin

#define LED_PIN 13

// Parsed command values
int l_dir = 0;
int l_pwm = 0;
int r_dir = 0;
int r_pwm = 0;

// Serial input buffer
char rx_buf[64];
uint8_t rx_index = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(SERIAL_BAUD);

   // --- Safe startup state ---
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(PWM_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  analogWriteFrequency(PWM_PIN, 25000);
  analogWriteFrequency(PWM_PIN2, 25000);
  analogWriteResolution(8);

  digitalWrite(PWM_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(PWM_PIN2, LOW);
  digitalWrite(DIR_PIN2, LOW);
  digitalWrite(LED_PIN, LOW);

  while (!Serial && millis() < 3000) {}

  Serial.println("Teensy wheel receiver ready");
}

void loop() {
  readSerial();
 
  setMotorSpeed(l_pwm, l_dir, r_pwm, r_dir);
  }


void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();


    if (c == '\n') {
      rx_buf[rx_index] = '\0';  // null terminate
      parseCommand(rx_buf);
      rx_index = 0;
    }
    
    else if (c != '\r') {
      if (rx_index < sizeof(rx_buf) - 1) {
        rx_buf[rx_index++] = c;
      } else {
        
        rx_index = 0;
      }
    }
  }
}

void parseCommand(const char *cmd) {
  // Expected format: l_dir,l_pwm,r_dir,r_pwm
  int values[4];

  int count = sscanf(cmd, "%d,%d,%d,%d",
                     &values[0], &values[1],
                     &values[2], &values[3]);

  if (count != 4) {
    Serial.print("Parse error: ");
    Serial.println(cmd);
    return;
  }

  l_dir = constrain(values[0], 0, 1);
  l_pwm = constrain(values[1], 0, 255);
  r_dir = constrain(values[2], 0, 1);
  r_pwm = constrain(values[3], 0, 255);

  // Debug (remove later)
  /*Serial.print("L: dir=");
  Serial.print(l_dir);
  Serial.print(" pwm=");
  Serial.print(l_pwm);
  Serial.print(" | R: dir=");
  Serial.print(r_dir);
  Serial.print(" pwm=");
  Serial.println(r_pwm);*/
  Serial.print("Left Motor");
  Serial.print(" speed set to ");
  Serial.println(l_pwm);

  Serial.print("Right Motor");
  Serial.print(" speed set to ");
  Serial.println(r_pwm);
}

// ---- Direct motor control (PWM + DIR) ----
void setMotorSpeed(int l_pwm, int l_dir, int r_pwm, int r_dir) {


  // ---- LEFT MOTOR ----
  if (l_dir == 0) {
    digitalWrite(DIR_PIN, LOW);
  } else {
    digitalWrite(DIR_PIN, HIGH);
  }

  analogWrite(PWM_PIN, l_pwm);


  // ---- RIGHT MOTOR ----
  if (r_dir == 0) {
    digitalWrite(DIR_PIN2, LOW);
  } else {
    digitalWrite(DIR_PIN2, HIGH);
  }

  analogWrite(PWM_PIN2, r_pwm);

}
