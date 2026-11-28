/*
 * Device 03: Probability & Statistics Logger (AI-Generated Baseline v1)
 * Hardware: Arduino Nano, KY-017 Tilt Sensor, Tactile Pushbutton, 0.96" OLED (SSD1306 via U8g2)
 * Features: 3 statistical modes (M1: Coin toss, M2: Two-dice sum distribution, M3: 500x Law of Large Numbers simulation),
 *           shake-to-roll trigger, short-press mode switch, and 1-second hold reset.
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Initialize 128x64 OLED display in page-buffer mode to save SRAM
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Hardware Pin Definitions
#define TILT_PIN 2   // KY-017 tilt/shake sensor (triggers a single random trial)
#define BTN_PIN 3    // Control button (D3): short click = switch mode, 1s hold = reset N

int mode = 1;
int lastTilt = 0;
int lastBtn = HIGH;
bool needRedraw = true;

unsigned long lastTrialTime = 0;
unsigned long btnPressStart = 0; // Timestamp when button press started
bool longPressHandled = false;   // Flag preventing duplicate long-press resets

long heads = 0;
long tails = 0;
int diceSums[13]; // Array storing frequencies for 2-dice sums (indices 2..12)

// Clear all statistical counters for the active mode (and run 500x batch in Mode 3)
void resetStats() {
  heads = 0;
  tails = 0;
  for (int i = 0; i <= 12; i++) diceSums[i] = 0;

  if (mode == 3) {
    for (int i = 0; i < 500; i++) {
      if (random(0, 2) == 0) heads++;
      else tails++;
    }
  }
}

// Execute a single random experiment based on the current mode
void makeTrial() {
  if (mode == 1) {
    if (random(0, 2) == 0) heads++;
    else tails++;
  } 
  else if (mode == 2) {
    int sum = random(1, 7) + random(1, 7);
    diceSums[sum]++;
  }
}

void setup() {
  u8g2.begin();
  pinMode(TILT_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  randomSeed(analogRead(A0)); // Seed PRNG using floating analog pin noise
  lastTilt = digitalRead(TILT_PIN);
}

void loop() {
  unsigned long now = millis();
  int currentBtn = digitalRead(BTN_PIN);

  // 1. Detect button press start
  if (lastBtn == HIGH && currentBtn == LOW) {
    btnPressStart = now;
    longPressHandled = false;
  }

  // 2. If button is held longer than 1 second (1000 ms), reset statistical counters
  if (currentBtn == LOW && !longPressHandled && (now - btnPressStart > 1000)) {
    longPressHandled = true;
    resetStats();
    needRedraw = true;
  }

  // 3. If button is released before 1 second (with 40 ms debounce), switch to next mode
  if (lastBtn == LOW && currentBtn == HIGH && !longPressHandled && (now - btnPressStart > 40)) {
    mode++;
    if (mode > 3) mode = 1;
    resetStats();
    needRedraw = true;
  }
  lastBtn = currentBtn;

  // 4. Poll KY-017 tilt sensor (1 shake = 1 trial, 250 ms cooldown)
  int currentTilt = digitalRead(TILT_PIN);
  if (lastTilt == LOW && currentTilt == HIGH && (now - lastTrialTime > 250)) {
    lastTrialTime = now;
    if (mode == 1 || mode == 2) {
      makeTrial();
      needRedraw = true;
    }
  }
  lastTilt = currentTilt;

  // 5. Render OLED screen only when data or mode changes
  if (needRedraw) {
    needRedraw = false;
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);

      if (mode == 1 || mode == 3) {
        long total = heads + tails;
        u8g2.setCursor(0, 10);
        if (mode == 1) u8g2.print("M1: Coin  N=");
        else u8g2.print("M3: 500x  N=");
        u8g2.print(total);

        float pHeads = (total > 0) ? (heads * 100.0 / total) : 0;
        float pTails = (total > 0) ? (tails * 100.0 / total) : 0;

        int hBar = (int)(pHeads * 0.35);
        int tBar = (int)(pTails * 0.35);

        u8g2.drawBox(20, 52 - hBar, 28, hBar);
        u8g2.drawBox(76, 52 - tBar, 28, tBar);
        u8g2.drawHLine(10, 52, 108);

        u8g2.setCursor(16, 63);
        u8g2.print(pHeads, 1);
        u8g2.print("%");

        u8g2.setCursor(72, 63);
        u8g2.print(pTails, 1);
        u8g2.print("%");
      } 
      else if (mode == 2) {
        // Sum all trials in Mode 2 to display total N and scale histogram bars
        long totalDice = 0;
        int maxVal = 1;
        for (int s = 2; s <= 12; s++) {
          totalDice += diceSums[s];
          if (diceSums[s] > maxVal) maxVal = diceSums[s];
        }

        u8g2.setCursor(0, 10);
        u8g2.print("M2: 2 Dice N=");
        u8g2.print(totalDice);

        for (int s = 2; s <= 12; s++) {
          int barHeight = map(diceSums[s], 0, maxVal, 0, 40);
          int xPos = 8 + (s - 2) * 10;
          u8g2.drawBox(xPos, 54 - barHeight, 7, barHeight);
        }
        u8g2.drawHLine(5, 54, 118);
        u8g2.setCursor(5, 63);
        u8g2.print("2     7     12");
      }
    } while (u8g2.nextPage());
  }
}
