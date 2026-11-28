# AI Prompt Log — Device 03: Probability & Statistics Logger

This log documents the iterative prompts used to generate the baseline firmware (`01-ai-baseline.ino`) and identifies the specific technical limitations I plan to resolve in my custom rewrite (`02-my-final-code.ino`).

---

## Prompt 1: Initial Concept & Hardware Architecture
> **Prompt:** *"Write an Arduino sketch for an Arduino Nano with a 0.96" I2C OLED display (U8g2 library), a KY-017 tilt sensor on D2, and a tactile button on D3. Create a portable probability and statistics logger with 3 modes: M1 (Coin toss triggered by shaking), M2 (Two-dice sum distribution from 2 to 12), and M3 (instant 500-trial simulation of the Law of Large Numbers). Short click switches modes, 1-second hold resets statistics."*

* **AI Output Summary:** Generated a single-file sketch using `U8G2_SSD1306_128X64_NONAME_1_HW_I2C` page-buffer rendering, `millis()`-based button press duration tracking, and `map()` scaling for bar charts.

---

## Prompt 2: Refining Display Counters & Array Bounds
> **Prompt:** *"In Mode 2 (2 Dice), also calculate and display the total number of trials `N=` at the top of the screen, and ensure the `diceSums` array prevents out-of-bounds memory access for sums up to 12."*

* **AI Output Summary:** Updated `diceSums` array size to `13` (indices `0..12`), added a `totalDice` summation loop in Mode 2, and added a `250 ms` cooldown timer on the KY-017 tilt sensor to reduce double-counting from mechanical bounce.

---

## Known Limitations in AI Baseline (Targets for `02-my-final-code.ino`)
1. **Volatile Memory Only:** All experiment data (`heads`, `tails`, `diceSums`) is lost on power-off; I plan to add **EEPROM wear-leveled storage** to save long-term classroom experiments.
2. **Mechanical Tilt Sensor Chatter:** A simple `250 ms` cooldown still occasionally misses rapid shakes or registers after-bounces; I will implement a **state-machine edge filter** and optional buzzer/LED feedback per registered trial.
3. **Pseudo-Random Seed Entropy:** `analogRead(A0)` provides limited entropy on startup; I will combine floating analog reads with hardware timer jitter (`micros()` at first button press) for stronger randomness.
