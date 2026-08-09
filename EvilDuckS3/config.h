#pragma once

#ifndef WIFI_SSID
#define WIFI_SSID "EvilDuck_S3"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "12345678"
#endif

#define SPIFFS_BASE_PATH "/"
#define DUCKSCRIPT_MAX_LINE_LEN 512
#define DEBUG_SERIAL_BAUD 115200

// ===== Status LED =====
// LilyGO T-Dongle-S3 uses an APA102 (clock+data, not single-wire WS2812).
// Confirmed pins from LilyGO docs: data=GPIO40, clock=GPIO39.
#define LED_ENABLED 1
#define LED_TYPE_NEOPIXEL 0
#define LED_TYPE_APA102 1
#define LED_APA102_DATA_PIN 40
#define LED_APA102_CLOCK_PIN 39
#define LED_NUM_PIXELS 1
#define LED_BRIGHTNESS 40
// Per-channel correction, 0-255.
#define LED_CORRECT_R 255
#define LED_CORRECT_G 255
#define LED_CORRECT_B 255
#define LED_USE_GAMMA 0
#define LED_PIN_R 25
#define LED_PIN_G 26
#define LED_PIN_B 27
#define LED_ANODE 0

// ===== microSD (SPI) — set SD_ENABLED 1 when wired =====
#define SD_ENABLED 38
#define SD_CS   10
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK  12
#define SD_SPI_FREQ_HZ 25000000
