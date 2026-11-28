# AI Prompt Log — Device 02: Ultrasonic Radar & Geometry Scanner

This log documents the iterative prompts used to generate the baseline firmware (`01-ai-baseline.ino`) and identifies the specific technical limitations I plan to resolve in my custom rewrite (`02-my-final-code.ino`).

---

## Prompt 1: Initial Concept & Radar Visualization
> **Prompt:** *"Write an Arduino sketch for an Arduino Nano with an HC-SR04 ultrasonic sensor (Trig D9, Echo D10), an SG90 micro servo (D6), and a 0.96" I2C OLED display using the U8g2 library. Sweep the servo from 15 to 165 degrees, map detected obstacles as dots inside a dual-arc radar screen, and display the current angle and distance in the top corners."*

* **AI Output Summary:** Generated a single-file sketch using `U8G2_SSD1306_128X64_NONAME_1_HW_I2C` page-buffer rendering to stay within the ATmega328P's 2 KB SRAM limit, polar-to-Cartesian coordinate conversion (`cos`/`sin`) for the sweep ray, and a 181-element `distances[]` array to persist obstacle blips across sweeps.

---

## Prompt 2: Extending Detection Range & Timeout Tuning
> **Prompt:** *"Increase the maximum radar range to 250 cm (2.5 meters), adjust the HC-SR04 echo timeout so distant reflections are not cut off, and format distances under 100 cm in centimeters while showing distances from 1.0 m to 2.5 m in meters."*

* **AI Output Summary:** Extended `pulseIn()` timeout to `30,000 µs` (`~5 m` round-trip acoustic window), increased servo stabilization pause to `40 ms`, and added conditional telemetry formatting (`cm` vs. `m` with one decimal place).

---

## Known Limitations in AI Baseline (Targets for `02-my-final-code.ino`)
1. **Blocking Execution (`delay(40)` + `pulseIn`):** Calling `delay(40)` and a `30 ms` blocking `pulseIn()` at every step causes choppy servo motion and freezes the UI; I plan to replace this with a **non-blocking `millis()` state machine** and hardware timer/interrupt echo capture.
2. **Single-Ping Acoustic Noise:** Relying on a single ultrasonic measurement per angle produces occasional false blips from stray reflections; I will implement a **3-sample median filter** before updating `distances[]`.
3. **No Geometric Dimension Estimation Yet:** Currently the scanner only plots raw polar points; in my custom rewrite I will add **angular edge detection and Law of Cosines / chord approximation** to estimate obstacle frontal width and cross-sectional area.
