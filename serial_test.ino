// Teensy 4.1 wheel command receiver
// Receives: l_dir,l_pwm,r_dir,r_pwm\n

#include <Arduino.h>

static const uint32_t SERIAL_BAUD = 115200;

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

  // Wait for serial (optional but helpful during debugging)
  while (!Serial && millis() < 3000) {}

  Serial.println("Teensy wheel receiver ready");
}

void loop() {
  readSerial();
  // applyMotorCommands();   // add later
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
}

void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();

    // End-of-line → process command
    if (c == '\n') {
      rx_buf[rx_index] = '\0';  // null terminate
      parseCommand(rx_buf);
      rx_index = 0;
    }
    // Ignore carriage return
    else if (c != '\r') {
      if (rx_index < sizeof(rx_buf) - 1) {
        rx_buf[rx_index++] = c;
      } else {
        // Buffer overflow protection
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

  // Debug echo (remove later)
  Serial.print("L: dir=");
  Serial.print(l_dir);
  Serial.print(" pwm=");
  Serial.print(l_pwm);
  Serial.print(" | R: dir=");
  Serial.print(r_dir);
  Serial.print(" pwm=");
  Serial.println(r_pwm);
}
