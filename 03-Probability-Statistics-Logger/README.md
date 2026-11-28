# Device 03: Probability & Statistics Logger

## 🎯 Overview
A portable hardware logger built for probability theory and statistics experiments. Uses a tilt sensor to trigger physical random trials and displays real-time bar charts and histograms on a `0.96"` OLED screen across three modes (`M1: Coin`, `M2: 2 Dice`, `M3: 500x`).

## 🔌 Wiring & Pinout (Arduino Nano)

| Component | Module Pin | Arduino Nano Pin | Function |
| :--- | :---: | :---: | :--- |
| **0.96" OLED (SSD1306)** | `VCC` | `5V` | Display power |
| | `GND` | `GND` | Common ground |
| | `SDA` | `A4` | I2C Data line |
| | `SCL` | `A5` | I2C Clock line |
| **KY-017 Tilt Sensor** | `VCC` | `5V` | Sensor power |
| | `GND` | `GND` | Common ground |
| | `DO (Signal)` | `D2` | Shake/tilt trigger input |
| **Tactile Pushbutton** | `Pin 1` | `D3` | Short click: switch mode; Hold 1s: reset `N` (`INPUT_PULLUP`) |
| | `Pin 2` | `GND` | Common ground |

## 📂 Files in This Module
* **[`01-ai-baseline.ino`](./01-ai-baseline.ino)** — Initial AI-generated code (3 statistical modes, `U8g2` page-buffer graphics, button hold-to-reset).
* **[`prompts.md`](./prompts.md)** — Exact prompt log used to generate and refine the baseline prototype.
* `02-my-final-code.ino` *(Coming in December)* — Custom rewrite with state-machine debouncing, EEPROM session saving, and improved PRNG seeding.
