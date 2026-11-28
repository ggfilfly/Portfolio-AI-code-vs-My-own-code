# Device 02: Ultrasonic Radar & Geometry Scanner

## 🔌 Wiring & Pinout (Arduino Nano)

| Component | Module Pin | Arduino Nano Pin | Function |
| :--- | :---: | :---: | :--- |
| **HC-SR04 Ultrasonic** | `VCC` | `5V` | Power supply (5V) |
| | `GND` | `GND` | Common ground |
| | `TRIG` | `D11` | Ultrasonic trigger pulse |
| | `ECHO` | `D12` | Echo return pulse |
| **SG90 Micro Servo** | `Red (VCC)` | `5V` | Servo power |
| | `Brown (GND)` | `GND` | Common ground |
| | `Orange (Signal)` | `D9` | PWM angle control (0–180°) |
| **0.96" OLED (SSD1306)** | `VCC` | `5V` | Display power |
| | `GND` | `GND` | Common ground |
| | `SDA` | `A4` | I2C Data line |
| | `SCL` | `A5` | I2C Clock line |

## 📂 Files in This Module
* **[`01-ai-baseline.ino`](./01-ai-baseline.ino)** — Initial AI-generated code (sweeps 0–180°, measures distance, plots obstacle blips).
* **[`prompts.md`](./prompts.md)** — Exact prompt log used to generate the baseline prototype.
* `02-my-final-code.ino` *(Coming in December)* — Custom non-blocking version with median noise filtering and area/perimeter calculation.
