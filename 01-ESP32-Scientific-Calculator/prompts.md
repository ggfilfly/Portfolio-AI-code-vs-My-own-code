# AI Prompt Log — Device 01: ESP32 Touchscreen Scientific Calculator

This log documents the iterative prompts used to generate the baseline firmware (`01-ai-baseline.ino`) and configure the dual SPI buses on the `ESP32-2432S028R` board, along with engineering targets for my custom rewrite (`02-my-final-code.ino`).

---

## Prompt 1: Touchscreen UI & Separate SPI Bus Architecture
> **Prompt:** *"Write an Arduino sketch for an ESP32-2432S028R ('Cheap Yellow Display', 2.8" 320x240 ILI9341 TFT + XPT2046 resistive touch) using `TFT_eSPI` and `XPT2046_Touchscreen` on a dedicated VSPI bus (`CLK 25, MISO 39, MOSI 32, CS 33, IRQ 36`). Draw a landscape 4x5 scientific calculator keypad with digits 0-9, basic operators (+, -, *, /), C, =, and degree-based functions (`sin`, `cos`, `tan`, `sqrt`)."*

* **AI Output Summary:** Generated a single-file GUI using `TFT_eSPI` rounded-rectangle buttons, explicit backlight activation on `GPIO 21`, dedicated `SPIClass(VSPI)` initialization for the `XPT2046` controller, and raw-to-screen coordinate mapping (`map()`).

---

## Prompt 2: Resolving Display Driver & Hardware Configuration
> **Prompt:** *"After uploading, the backlight turns on but the screen remains white. Provide the exact `User_Setup.h` pinout and driver configuration for the ESP32-2432S028R board."*

* **AI Output Summary:** Configured `ILI9341_2_DRIVER` with HSPI pins (`MISO 12, MOSI 13, SCLK 14, CS 15, DC 2, RST -1, BL 21`), resolving the white-screen issue and rendering the full touchscreen keypad.

---

## Known Limitations in AI Baseline (Targets for `02-my-final-code.ino`)
1. **Sequential Two-Operand Logic Only:** Currently evaluates operations sequentially (`A op B =`) without operator precedence or parentheses; I plan to implement a **Shunting-Yard expression parser** so full formulas like `2 + 3 * sin(30)` evaluate correctly.
2. **Blocking Touch Debounce (`delay(220)`):** Uses a blocking `delay(220)` after each stylus tap; I will replace this with a **non-blocking `millis()` state machine** and visual button press highlight feedback.
3. **No Function Graphing or Battery Telemetry:** I plan to add a **2D function plotter mode** (`y = f(x)`) for algebra/SAT prep and an **ADC battery voltage indicator** when powered by the `18650` cell.
