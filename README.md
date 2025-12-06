# Colorful Game of Life - Arduino TFT Edition

This project implements a **colorful version of Conway's Game of Life** using Arduino and TFT screens with the `TFT_eSPI` library. Each cell is represented with **8-bit RGB colors per channel**, and the grid evolves **bitwise per channel**, producing vibrant and dynamic patterns.

![Example of colorful patterns](screenshot.png)

---

## Features

- **Real-time simulation** on Arduino with a TFT display.  
- **RGB-colored cells** (8 bits per channel) for rich visual effects.  
- **Customizable rules** using a 32-bit integer (`RULE`) to define birth and survival per bit.  
- **Toroidal grid**: edges wrap around for an infinite effect.  
- Adjustable parameters:
  - Grid size (`GRID_W` x `GRID_H`)  
  - Cell size (`CELL_SIZE`)  
  - Initial life probability (`LIFE_INIT`)  

---

## Requirements

- Arduino IDE or compatible platform (ESP8266 or ESP32 recommended)  
- [`TFT_eSPI` library](https://github.com/Bodmer/TFT_eSPI)  
- TFT display compatible with `TFT_eSPI`  
- Correct SPI wiring for the display  

---

## Simulation settings

- const uint8_t GRID_W = 32;      // grid width
- const uint8_t GRID_H = 40;      // grid height
- const uint8_t CELL_SIZE = 4;    // cell size in pixels
- const uint8_t LIFE_INIT = 30;   // percentage of live cells at start
- const uint32_t RULE = 0x1808;   // bitwise birth/survival rule
