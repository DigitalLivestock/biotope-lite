# BIOTOPE(LITE)™

> BIOTOPE(LITE)™: a High Precision Environment Monitoring Unit

![GitHub License](https://img.shields.io/github/license/DigitalLivestock/biotope-lite)
![GitHub Release](https://img.shields.io/github/v/release/DigitalLivestock/biotope-lite)

---

## A Bit of History
I started this project a couple of years ago and almost finished it, but life got in the way. Recently, I decided it was time to tie up the loose ends and complete it. While the initial development was paused, the recent work focused on finalizing the essential functionality. Some cleaning of code and its structure was performed, but some of the hacky parts still remain. The main goal of the project was to build an embedded solution based on an AVR microcontroller, designing and implementing as much of the supporting electronics from scratch as possible.

---

## About

The BIOTOPE(LITE)™ is a high precision monitoring unit for measuring indoor and outdoor temperature, relative humidity and pressure. Measurements are displayed on the built-in LCD screen, presenting current, maximum and minimum for data with smart backlight. Navigation among data is enabled by available buttons. Measured maximum and minimum are automatically restored between power on and -off. Through available settings, it is possible to manually store and reset data.

<img width="2874" height="1828" alt="IMG_20260415_201508294" src="https://github.com/user-attachments/assets/ad92668c-d06f-4ae4-81f3-be026136bac8" />

## Features

* Indoor temperature reading in range (-)40 – 85 °C.
* Indoor pressure readings in range 300 – 1100 hPa.
* Indoor relative humidity as 0 – 100 %.
* Outdoor temperature reading in range (-)25 – 125 °C.
* Minimum and maximum tracking of temperature, humidity and pressure.
* LCD display with timer based backlight.
* Buttons for navigation and configuration.
* Persistent data store and restore between power states.
* Ultra low power consumption at 15mA when idle or 23 mA when active.
* Battery (4 x AA) or wall powered (0.2A/6V via 2.1 mm DC-Jack).

---

## Usage
Choose if you want to use 4 x AA batteries or a compatible wall power adapter capable of supplying at least 0.2A at 6V via DC-Jack (2.1 mm):
    • Battery powered: carefully open BIOTOPE(LITE)™ by unscrewing the back panel and inserting 4 x AA batteries into the battery holder.
    • Wall power adapter: connect adapter DC-Jack male contact to BIOTOPE(LITE)™ female.
Power-on device by moving switch above DC-Jack female connector. The LCD screen shall light-up showing initial start-up screen. 

The device displays measurement data and configuration options on the LCD screen in the form of frames, among which a user can navigate using buttons. As can be seen in the next figure below, a frame is made up of two rows and there are five buttons available. Buttons consist of UP, DOWN, LEFT, RIGHT, and ENTER—allowing for full access among network of selectable frames, as can be observed in the second figure. Frames can be seen connected to one another via arrows, here representing button actions.

<img width="832" height="204" alt="image" src="https://github.com/user-attachments/assets/b6328d5e-4d43-4e8e-ab90-4c9be72bbafa" />

In the following, each frame and their functions are explained more in detail. The identification numbers of each Frame are found in the green boxes inserted at top-right of each frame in the following figure.
* Frame 0.0: Initial – Device name and software version 
* Frame 1.0: Current temperature indoors and outdoors (°C)
* Frame 1.1: Maximum and minimum temperature indoors (°C)
* Frame 1.2: Maximum and minimum temperature outdoors (°C)
* Frame 2.0: Current relative humidity (%) and pressure (hPa) indoors
* Frame 2.1: Maximum and minimum relative humidity indoors (%)
* Frame 2.2: Maximum and minimum pressure indoors (hPa)
* Frame 3.0: Settings menu
* Frame 3.1: Backup all maximum and minimum data using persistent storage when pressing ENTER-button
* Frame 3.2: Reset all maximum and minimum data in persistent storage when pressing ENTER-button

<img width="1150" height="664" alt="image" src="https://github.com/user-attachments/assets/38022ff3-c9e3-4d0e-9758-e5b38cd03146" />

---

## Project Structure

```
biotope-lite/
├── src/
│ └── biotope_lite/
│     ├── biotope_lite.ino
│     ├── biotope_lite_app.h
│     ├── biotope_lite_app.cpp
│     ├── config.h
│     ├── lcd.h 
│     ├── lcd.cpp
│     ├── bme280.h 
│     ├── bme280.cpp
│     ├── ntc.h 
│     ├── ntc.cpp  
│     ├── MCP23016.h
│     ├── MCP23016.cpp
│     ├── twi.h
│     ├── twi.cpp
│     ├── queue.h
│     ├── queue.cpp 
│     └── biotope_manual_rev1.0.odt
├── hardware/
│ └── schematic/
│   ├── biotope_lite_rev1.0.kicad_prl
│   ├── biotope_lite_rev1.0.kicad_pro
│   └── biotope_lite_rev1.0.kicad_sch
├── docs/
│ ├── figures/
│ └── biotope_manual_rev1.0.odt
├── biotope_lite_manual_v1.0.pdf
├── biotope_lite_schematic_rev1.0.pdf
├── LICENSE.md
└── README.md
```

---

## License

This project is licensed under the MIT License – see the LICENSE file for details.

---
