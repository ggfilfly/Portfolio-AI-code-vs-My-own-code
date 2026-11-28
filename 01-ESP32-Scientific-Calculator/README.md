# Device 01: ESP32 Touchscreen Scientific Calculator

## 🎯 Overview
An autonomous handheld scientific calculator built on the `ESP32-2432S028R` ("Cheap Yellow Display") featuring a `2.8"` `320x240` TFT touchscreen interface (`ILI9341` + `XPT2046`) operated via stylus. Supports basic arithmetic (`+`, `-`, `*`, `/`) and degree-based scientific functions (`sin`, `cos`, `tan`, `sqrt`).

## 🔌 Hardware & Pinout (`ESP32-2432S028R` Integrated Board)

| Subsystem | Interface | ESP32 GPIO Pins | Function |
| :--- | :---: | :---: | :--- |
| **TFT Display (`ILI9341`)** | HSPI | `MISO: 12`, `MOSI: 13`, `SCLK: 14`, `CS: 15`, `DC: 2`, `RST: -1` | Driven via `TFT_eSPI` (`ILI9341_2_DRIVER`) |
| **Backlight Control** | Digital | `GPIO 21` | Active `HIGH` display backlight |
| **Touch Controller (`XPT2046`)** | VSPI | `CLK: 25`, `MISO: 39`, `MOSI: 32`, `CS: 33`, `IRQ: 36` | Dedicated SPI bus via `XPT2046_Touchscreen` |
| **Power Supply (Planned)** | 5V Boost | `5V / GND` | `18650` Li-Ion cell + `TP4056` + `MT3608` |

## 📂 Files in This Module
* **[`01-ai-baseline.ino`](./01-ai-baseline.ino)** — Initial AI-generated touchscreen UI (`4x5` keypad, arithmetic, trigonometry in degrees, `sqrt`).
* **[`prompts.md`](./prompts.md)** — Exact prompt log used to generate the baseline firmware and configure `TFT_eSPI`.
* `02-my-final-code.ino` *(Coming in December)* — Custom rewrite with multi-step expression parser (order of operations), parentheses, graph plotting mode, and battery voltage monitor.
