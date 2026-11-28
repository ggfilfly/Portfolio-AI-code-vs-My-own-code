# Device 02: Ultrasonic Radar & Geometry Scanner

## 🎯 Overview
A portable acoustic radar and geometry scanner built on an Arduino Nano. Sweeps a servo-mounted ultrasonic transducer from `15°` to `165°` up to `2.5 m` and renders a real-time dual-arc polar radar map with live angle/distance telemetry on a `0.96"` OLED screen.

## 🔌 Wiring & Pinout (Arduino Nano)

| Component | Module Pin | Arduino Nano Pin | Function |
| :--- | :---: | :---: | :--- |
| **0.96" OLED (SSD1306)** | `VCC` | `5V` | Display power |
| | `GND` | `GND` | Common ground |
| | `SDA` | `A4` | I2C Data line |
| | `SCL` | `A5` | I2C Clock line |
| **HC-SR04 Ultrasonic** | `VCC` | `5V` | Sensor power |
| | `GND` | `GND` | Common ground |
| | `TRIG` | `D9` | Ultrasonic trigger pulse |
| | `ECHO` | `D10` | Echo return pulse |
| **SG90 Micro Servo** | `Red (VCC)` | `5V` | Servo power |
| | `Brown (GND)` | `GND` | Common ground |
| | `Orange (Signal)` | `D6` | PWM angle control (`15°–165°`) |

## 📂 Files in This Module
* **[`01-ai-baseline.ino`](./01-ai-baseline.ino)** — Initial AI-generated code (`15°–165°` sweep, `250 cm` range, `U8g2` page-buffer graphics).
* **[`prompts.md`](./prompts.md)** — Exact prompt log used to generate and refine the baseline prototype.
* `02-my-final-code.ino` *(Coming in December)* — Custom rewrite with non-blocking `millis()` sweep, 3-sample median noise filter, and obstacle width/area calculation.
