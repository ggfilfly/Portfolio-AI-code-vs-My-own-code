/*
 * Device 02: Ultrasonic Radar Scanner (AI-Generated Baseline v1)
 * Hardware: Arduino Nano, HC-SR04 Ultrasonic Sensor, SG90 Servo, 0.96" OLED (SSD1306)
 * Current Functionality: Sweeps 0-180 degrees, measures distance (cm), and plots radar map with obstacle points.
 */

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Settings (128x64 I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Hardware Pins
#define TRIG_PIN 11
#define ECHO_PIN 12
#define SERVO_PIN 9

// Radar Constants
#define MAX_DIST_CM 100.0   // Maximum detection range (cm)
#define MIN_DIST_CM 2.0     // Minimum valid reading (cm)
#define STEP_ANGLE 3        // Servo step in degrees

Servo radarServo;

void setup() {
  Serial.begin(9600);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  radarServo.attach(SERVO_PIN);
  radarServo.write(0);
  
  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 25);
  display.println(F("ULTRASONIC RADAR"));
  display.setCursor(22, 38);
  display.println(F("INITIALIZING..."));
  display.display();
  delay(1000);
}

// Measure distance using HC-SR04 with timeout protection
float getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Timeout 15000 us (~250 cm max) prevents freezing on open space
  long duration = pulseIn(ECHO_PIN, HIGH, 15000);
  if (duration == 0) {
    return -1.0; // Out of range
  }
  
  float distance = duration * 0.0343 / 2.0;
  if (distance < MIN_DIST_CM || distance > MAX_DIST_CM) {
    return -1.0;
  }
  return distance;
}

// Draw radar grid, sweep ray, detected obstacle points, and angle/distance telemetry
void updateRadarScreen(int angle, float dist) {
  // Clear radar area at the end of each 180-degree sweep
  if (angle == 0 || angle == 180) {
    display.clearDisplay();
  }
  
  int centerX = 64;
  int centerY = 63;
  int maxRadius = 48;
  
  // Draw semi-circular radar arcs and baseline
  display.drawCircle(centerX, centerY, maxRadius, SSD1306_WHITE);
  display.drawCircle(centerX, centerY, maxRadius / 2, SSD1306_WHITE);
  display.drawLine(centerX - maxRadius, centerY, centerX + maxRadius, centerY, SSD1306_WHITE);
  
  // Convert polar coordinates (angle, dist) to Cartesian screen coordinates
  float rad = angle * PI / 180.0;
  int lineX = centerX - cos(rad) * maxRadius;
  int lineY = centerY - sin(rad) * maxRadius;
  
  // Draw current sweep ray
  display.drawLine(centerX, centerY, lineX, lineY, SSD1306_WHITE);
  
  // Plot obstacle point if valid distance detected
  if (dist > 0) {
    float scaledR = (dist / MAX_DIST_CM) * maxRadius;
    int objX = centerX - cos(rad) * scaledR;
    int objY = centerY - sin(rad) * scaledR;
    display.fillCircle(objX, objY, 2, SSD1306_WHITE);
  }
  
  // Top telemetry header (clear top 14 pixels first)
  display.fillRect(0, 0, 128, 14, SSD1306_BLACK);
  display.setCursor(4, 2);
  display.print(F("Angle: "));
  display.print(angle);
  display.print(F(" deg"));
  
  display.setCursor(74, 2);
  display.print(F("Dist: "));
  if (dist > 0) {
    display.print((int)dist);
    display.print(F("cm"));
  } else {
    display.print(F("--"));
  }
  
  display.display();
}

void processStep(int angle) {
  radarServo.write(angle);
  delay(35); // Wait for servo stabilization (known AI baseline bottleneck)
  
  float dist = getDistanceCM();
  updateRadarScreen(angle, dist);
}

void loop() {
  // Forward sweep: 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += STEP_ANGLE) {
    processStep(angle);
  }
  
  // Reverse sweep: 180 down to 0 degrees
  for (int angle = 180; angle >= 0; angle -= STEP_ANGLE) {
    processStep(angle);
  }
}
