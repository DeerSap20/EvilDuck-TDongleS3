<div align="center">

  <img src="https://raw.githubusercontent.com/DeerSap20/EvilDuck-TDongleS3/refs/heads/main/banner.svg" alt="EvilDuck Banner" width="100%"/>

  <br/>
  <br/>

  <!-- Badges -->
  <a href="https://github.com/cifertech/evilduck">
    <img src="https://img.shields.io/static/v1?label=cifertech&message=evilduck&color=orange&logo=github" alt="cifertech - EvilDuck"/>
  </a>
  &nbsp;
  <a href="https://github.com/cifertech/evilduck">
    <img src="https://img.shields.io/github/stars/cifertech/evilduck?style=social" alt="Stars"/>
  </a>
  &nbsp;
  <a href="https://github.com/cifertech/evilduck">
    <img src="https://img.shields.io/github/forks/cifertech/evilduck?style=social" alt="Forks"/>
  </a>
  &nbsp;
  <img src="https://img.shields.io/badge/ESP32--S3-WiFi%20%2B%20USB%20HID-orange?logo=espressif" alt="ESP32-S3"/>
  &nbsp;
  <img src="https://img.shields.io/badge/license-MIT-orange" alt="MIT License"/>

  <br/>
  <br/>

  <p>
  <a href="https://twitter.com/techcifer"><img src="https://img.shields.io/badge/Twitter-orange?logo=x&logoColor=black"/></a>
  <a href="https://www.instagram.com/cifertech/"><img src="https://img.shields.io/badge/Instagram-orange?logo=instagram&logoColor=black"/></a>
  <a href="https://www.youtube.com/@techcifer"><img src="https://img.shields.io/badge/YouTube-orange?logo=youtube&logoColor=black"/></a>
  <a href="https://cifertech.net/"><img src="https://img.shields.io/badge/Website-orange?logo=googlechrome&logoColor=black"/></a>
</p>

</div>

<br/>



# EvilDuck S3 — LilyGO T-Dongle-S3 Port

This is a fork of [cifertech/EvilDuck](https://github.com/cifertech/EvilDuck), adapted to run on the [**LilyGO T-Dongle-S3**](https://lilygo.cc/en-us/products/t-dongle-s3) — a USB-A dongle-form-factor ESP32-S3 board with a built-in 0.96" ST7735 display and APA102 RGB LED.

The original EvilDuckS3 firmware was written for CiferTech's custom PCB. This fork swaps out the display and LED drivers to match the T-Dongle-S3's actual hardware, and fixes a Windows-only build bug.

## What's different from upstream

| Component | Original (CiferTech PCB) | This fork (T-Dongle-S3) |
|---|---|---|
| Display | None | ST7735 SPI, 80x160 — shows WiFi AP name, IP, uptime, free storage, battery |
| Status LED | WS2812/NeoPixel (single-wire) | APA102/DotStar (clock+data) |
| `spiffs.h`/`spiffs.cpp` | — | Renamed to `fs_spiffs.h`/`fs_spiffs.cpp` to avoid a Windows case-insensitive filename collision with the ESP32 core's own `SPIFFS.h` |

## Hardware — LilyGO T-Dongle-S3 confirmed pinout

| Function | GPIO |
|---|---|
| Display CS | 4 |
| Display MOSI | 3 |
| Display CLK | 5 |
| Display DC | 2 |
| Display RESET | 1 |
| Display Backlight | 38 (active LOW) |
| LED Data (APA102) | 40 |
| LED Clock (APA102) | 39 |
| Button | 0 |

**Note on the microSD/TF slot:** LilyGO's own listing states the built-in "virtual TF card" has no actual memory/function on this board. If you need SD storage, use an SD-equipped variant or fall back to internal flash (SPIFFS) storage mode, which this firmware supports.

## Requirements

- [Arduino IDE 2.x](https://www.arduino.cc/en/software)
- ESP32 board package (Boards Manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`)
- Board profile: **ESP32S3 Dev Module**
- **USB CDC On Boot: Enabled** (Tools menu) — required for native USB on this board

### Libraries (install via Library Manager unless noted)

- **GFX Library for Arduino** (by moononournation) — drives the ST7735 display
- **Adafruit DotStar** — drives the APA102 LED
- **ESP Async WebServer** (ESP32Async fork — search "ESP Async WebServer", or manually install from [github.com/ESP32Async/ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer))
- **AsyncTCP** (same ESP32Async org — [github.com/ESP32Async/AsyncTCP](https://github.com/ESP32Async/AsyncTCP)) — must match the ESPAsyncWebServer source to avoid version-mismatch compile errors

> Install ESPAsyncWebServer and AsyncTCP from the **same** source/maintainer. Mixing forks (e.g. an old ESPAsyncWebServer with a newer AsyncTCP) causes `const`-qualifier compile errors.

## Flashing

1. Open `EvilDuckS3/EvilDuckS3.ino` in Arduino IDE.
2. Select **Tools → Board → ESP32S3 Dev Module**.
3. Select **Tools → USB CDC On Boot → Enabled**.
4. Plug in the T-Dongle-S3. If it doesn't appear under **Tools → Port**, hold the board's button while plugging in to force download mode, then release after ~2 seconds.
5. Click **Upload**.

On first boot you'll see a brief boot logo, then the status screen (WiFi AP name, IP address, uptime, free storage, battery voltage if wired).

## Display

The built-in 0.96" screen shows a live status readout, refreshed once per second:

| Line | Shows |
|---|---|
| 1 | WiFi AP name (SSID) — the network name to connect to for the web control panel |
| 2 | IP address of the device's own access point (typically `192.168.4.1`) |
| 3 | Uptime since last boot (`HH:MM:SS`) |
| 4 | Free internal storage (SPIFFS), in KB |
| 5 | Battery voltage, if a battery ADC pin is wired (`N/A` otherwise) |

On power-up, it briefly shows a boot logo before switching to the status screen. This is purely informational — the display doesn't show anything about scripts or execution state, and there's no on-screen menu or selection UI.

## Troubleshooting

- **Blank screen after flashing:** double-check the SPI pins in `display.cpp` match the table above, and confirm the GFX library installed correctly.
- **Board flickers in/out of the port list:** this happened during development whenever a firmware build crashed early in `setup()` (e.g. a display init hang) — the board reboots in a loop, dropping USB each time. Forcing download mode via the button and reflashing known-good code resolves it.
- **`SPIFFS`/`File` "not declared in this scope" (Windows only):** make sure you're using this fork's `fs_spiffs.h`/`fs_spiffs.cpp`, not the upstream `spiffs.h`/`spiffs.cpp` — the rename avoids a case-insensitive filename collision with the ESP32 core's real `SPIFFS.h` library.

## Credits

All core firmware logic (web UI, DuckScript interpreter, HID backend, storage) is from the original [cifertech/EvilDuck](https://github.com/cifertech/EvilDuck) project. This fork only adapts the hardware layer for the T-Dongle-S3.

## License

MIT — see [LICENSE](LICENSE), inherited from the upstream project.
 
## 🤝 Contact
 
▶ Support me on Patreon: [patreon.com/cifertech](https://www.patreon.com/cifertech)
 
CiferTech — [@twitter](https://twitter.com/techcifer) — CiferTech@gmail.com
 
Project Link: [https://github.com/cifertech/EvilDuck](https://github.com/cifertech/EvilDuck)


