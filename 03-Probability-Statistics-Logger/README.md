# Device 03: Probability & Statistics Logger

## 🎯 Overview
A portable hardware logger designed for real-world probability experiments and statistics classes (recording trial outcomes, calculating empirical frequencies, and displaying live distributions on an OLED screen).

## 🔌 Wiring & Pinout (Arduino Nano)

| Component | Module Pin | Arduino Nano Pin | Function |
| :--- | :---: | :---: | :--- |
| **0.96" OLED (SSD1306)** | `VCC` | `5V` | Display power |
| | `GND` | `GND` | Common ground |
| | `SDA` | `A4` | I2C Data line |
| | `SCL` | `A5` | I2C Clock line |
| **Tactile Buttons (Event A / B / Reset)** | `Button 1` | `D2` | Log Outcome A (`INPUT_PULLUP`) |
| | `Button 2` | `D3` | Log Outcome B (`INPUT_PULLUP`) |
| | `Button 3` | `D4` | Reset / Mode Switch (`INPUT_PULLUP`) |
| **Potentiometer (Optional Threshold)** | `OUT` | `A0` | Adjust sample size / distribution view |

*(Note: Exact digital pins `D2–D4` can be adjusted in `01-ai-baseline.ino` to match your final enclosure layout).*

## 📂 Files in This Module
* **[`01-ai-baseline.ino`](./01-ai-baseline.ino)** — Initial AI-generated code (3 statistical modes, U8g2 page-buffer graphics, button hold-to-reset).
* **[`prompts.md`](./prompts.md)** — Exact prompt log used to generate and refine the baseline prototype.
* `02-my-final-code.ino` *(Coming in December)* — Custom rewrite with state-machine debouncing, EEPROM session saving, and improved PRNG seeding.
