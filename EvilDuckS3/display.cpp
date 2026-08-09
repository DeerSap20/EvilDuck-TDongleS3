#include "display.h"
#include "config.h"
#include "settings.h"
#include "fs_spiffs.h"
#include "debug.h"

#include <WiFi.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

// LilyGO T-Dongle-S3 confirmed pinout (ST7735, SPI, 80x160)
#define TFT_CS    4
#define TFT_MOSI  3
#define TFT_SCLK  5
#define TFT_DC    2
#define TFT_RST   1
#define TFT_BL    38   // backlight, active LOW per LilyGO docs

// Set to a real ADC pin if your board exposes battery voltage; -1 disables it.
#ifndef DISPLAY_BATTERY_PIN
#define DISPLAY_BATTERY_PIN -1
#endif
#define DISPLAY_BATTERY_DIVIDER 2.0f

static Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED);
static Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 3 /* rotation */, true /* IPS */,
                                              80, 160, 26, 1, 26, 1);

static unsigned long lastUpdateMs = 0;

static float readBatteryVoltage() {
  if (DISPLAY_BATTERY_PIN < 0) return -1.0f;
  int raw = analogRead(DISPLAY_BATTERY_PIN);
  return (raw / 4095.0f) * 3.3f * DISPLAY_BATTERY_DIVIDER;
}

static String formatUptime(unsigned long ms) {
  unsigned long totalSeconds = ms / 1000;
  unsigned long hours = totalSeconds / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;
  unsigned long seconds = totalSeconds % 60;
  char buf[16];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu", hours, minutes, seconds);
  return String(buf);
}

namespace display {

void begin() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); // active LOW backlight - LOW turns it on

  if (DISPLAY_BATTERY_PIN >= 0) {
    pinMode(DISPLAY_BATTERY_PIN, INPUT);
  }

  gfx->begin();
  gfx->fillScreen(0x0000);

  gfx->setTextColor(0xFFFF);
  gfx->setTextSize(1);
  gfx->setCursor(4, 40);
  gfx->println("EvilDuck S3");
  gfx->setCursor(4, 55);
  gfx->println("booting...");

  debug::info("Display initialized");
}

void update() {
  unsigned long now = millis();
  if (now - lastUpdateMs < 1000) return;
  lastUpdateMs = now;

  IPAddress ip = WiFi.softAPIP();
  String ssid = settings::getSSID();
  String uptimeStr = formatUptime(now);
  float batteryV = readBatteryVoltage();
  size_t freeBytes = spiffs::freeBytes();

  gfx->fillScreen(0x0000);
  gfx->setTextColor(0xFFFF);
  gfx->setTextSize(1);

  gfx->setCursor(2, 2);
  gfx->println(ssid);

  gfx->setCursor(2, 16);
  gfx->print("IP: ");
  gfx->println(ip.toString());

  gfx->setCursor(2, 30);
  gfx->print("Up: ");
  gfx->println(uptimeStr);

  gfx->setCursor(2, 44);
  gfx->print("Free: ");
  gfx->print(freeBytes / 1024.0f, 0);
  gfx->println(" KB");

  gfx->setCursor(2, 58);
  if (batteryV >= 0) {
    gfx->print("Batt: ");
    gfx->print(batteryV, 2);
    gfx->println(" V");
  } else {
    gfx->println("Batt: N/A");
  }
}

}
