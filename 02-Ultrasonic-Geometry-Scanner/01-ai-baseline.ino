/*
 * Device 02: Ultrasonic Radar Scanner (AI-Generated Baseline v1)
 * Hardware: Arduino Nano, HC-SR04 Ultrasonic Sensor, SG90 Servo, 0.96" OLED (SSD1306 via U8g2)
 * Features: Scans 15-165 degrees up to 2.5 m, stores obstacle map in array,
 *           and renders dual-arc radar with live angle/distance telemetry.
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Servo.h>

// Initialize 128x64 OLED display in page-buffer mode to save SRAM
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Servo radarServo;

// Hardware Pin Definitions
#define TRIG_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 6
#define MAX_DIST 250 // Maximum radar detection range in centimeters (2.5 meters)

int distances[181]; // Array storing measured distances for each angle
int currentAngle = 15;
int stepDir = 3;

// Measure distance in cm using HC-SR04 with extended echo timeout
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Extended pulseIn timeout to 30,000 us so ultrasonic echo returns from 2.5-3 meters
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float dist = duration * 0.0343 / 2.0;
  
  if (dist <= 2 || dist > MAX_DIST) return -1;
  return dist;
}

void setup() {
  u8g2.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  radarServo.attach(SERVO_PIN);
  
  // Clear obstacle map array on startup
  for (int i = 0; i <= 180; i++) distances[i] = -1;
}

void loop() {
  radarServo.write(currentAngle);
  delay(40); // Pause to allow servo stabilization and distant echo reception

  // Store measured distance across a 3-degree sector for smoother obstacle blips
  int d = (int)getDistance();
  for (int a = currentAngle - 1; a <= currentAngle + 1; a++) {
    if (a >= 0 && a <= 180) distances[a] = d;
  }

  // Render radar graphics using U8g2 page loop
  u8g2.firstPage();
  do {
    // Semi-circular range arcs: inner = 1.25 m, outer = 2.5 m
    u8g2.drawCircle(64, 63, 30, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
    u8g2.drawCircle(64, 63, 60, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);

    // Current radar sweep ray
    float rad = currentAngle * 3.14159 / 180.0;
    int lx = 64 - (int)(60 * cos(rad));
    int ly = 63 - (int)(60 * sin(rad));
    u8g2.drawLine(64, 63, lx, ly);

    // Plot detected obstacle points scaled up to 250 cm
    for (int a = 15; a <= 165; a += 3) {
      if (distances[a] > 0) {
        float r = map(distances[a], 0, MAX_DIST, 0, 60);
        float aRad = a * 3.14159 / 180.0;
        int px = 64 - (int)(r * cos(aRad));
        int py = 63 - (int)(r * sin(aRad));
        u8g2.drawDisc(px, py, 2);
      }
    }

    // Top-left telemetry: current servo angle
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.setCursor(0, 8);
    u8g2.print(currentAngle);
    u8g2.print("deg");

    // Top-right telemetry: distance in cm (< 1 m) or meters (>= 1 m)
    u8g2.setCursor(92, 8);
    if (d > 0 && d < 100) {
      u8g2.print(d);
      u8g2.print("cm");
    } else if (d >= 100) {
      u8g2.print(d / 100.0, 1);
      u8g2.print("m");
    } else {
      u8g2.print("---");
    }
  } while (u8g2.nextPage());

  // Reverse sweep direction at 15 and 165 degree limits
  currentAngle += stepDir;
  if (currentAngle >= 165 || currentAngle <= 15) {
    stepDir = -stepDir;
  }
}
