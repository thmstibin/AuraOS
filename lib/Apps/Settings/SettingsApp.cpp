#include "SettingsApp.h"
#include "AppManager.h"
#include "PowerManager.h"
#include "SettingsManager.h"
#include "DisplayManager.h"
#include "Colors.h"
#include "Config.h"
#include <Arduino.h>

static const int16_t ROW_H  = 52;
static const int16_t ROW_X  = 16;
static const int16_t FIRST_Y = STATUS_BAR_H + 52;

void SettingsApp::begin(Arduino_GFX *gfx) {
    _gfx    = gfx;
    _dirty  = true;
    _backBtn = Button(8, STATUS_BAR_H + 6, 64, 34, "< Back");
    _backBtn.setColors(Colors::Overlay, Colors::TextPrimary);
}

void SettingsApp::update() {}

void SettingsApp::_drawRow(int16_t row, const char *label, const char *value) const {
    int16_t y = FIRST_Y + row * ROW_H;
    _gfx->fillRect(ROW_X, y, SCREEN_W - ROW_X * 2, ROW_H - 2, Colors::Card);
    _gfx->drawRoundRect(ROW_X, y, SCREEN_W - ROW_X * 2, ROW_H - 2, 8, Colors::Overlay);
    _gfx->setTextColor(Colors::TextPrimary);
    _gfx->setTextSize(2);
    _gfx->setCursor(ROW_X + 12, y + 10);
    _gfx->print(label);
    _gfx->setTextColor(Colors::TextSecond);
    _gfx->setTextSize(1);
    _gfx->setCursor(ROW_X + 12, y + 34);
    _gfx->print(value);
}

void SettingsApp::draw() {
    if (!_dirty) return;
    _dirty = false;

    auto &pm = PowerManager::instance();
    auto &sm = SettingsManager::instance();
    auto &s  = sm.settings();

    _gfx->fillScreen(Colors::Black);
    _statusBar.draw(_gfx, pm.batteryPercent(), pm.isCharging(), false);

    // Title
    _gfx->setTextColor(Colors::White);
    _gfx->setTextSize(2);
    _gfx->setCursor(SCREEN_W / 2 - 42, STATUS_BAR_H + 10);
    _gfx->print("Settings");

    _backBtn.draw(_gfx);

    char buf[16];
    snprintf(buf, sizeof(buf), "%d%%", s.brightness * 100 / 255);
    _drawRow(0, "Brightness", buf);
    _drawRow(1, "Dark Mode",  s.darkMode    ? "On"  : "Off");
    _drawRow(2, "Wi-Fi",      s.wifiEnabled ? "On"  : "Off");
    _drawRow(3, "Bluetooth",  s.bleEnabled  ? "On"  : "Off");
    snprintf(buf, sizeof(buf), "%d%%", s.volume);
    _drawRow(4, "Volume", buf);
}

bool SettingsApp::onTouch(TouchEvent ev, int16_t x, int16_t y) {
    if (ev == TouchEvent::Tap) {
        if (_backBtn.contains(x, y)) {
            AppManager::instance().pop();
            return true;
        }
        // Row hit test for brightness slider
        if (x >= ROW_X && x < SCREEN_W - ROW_X) {
            int16_t rel = y - FIRST_Y;
            if (rel >= 0) {
                int row = rel / ROW_H;
                auto &s = SettingsManager::instance().settings();

                if (row == 0) {
                    // Brightness: tap right side to increase, left to decrease
                    int16_t colX = x - ROW_X;
                    if (colX < (SCREEN_W - ROW_X * 2) / 2) {
                        s.brightness = constrain(s.brightness - 20, 50, 255);
                    } else {
                        s.brightness = constrain(s.brightness + 20, 50, 255);
                    }
                    DisplayManager::instance().setBrightness(s.brightness);
                } else if (row >= 1 && row <= 4) {
                    // Toggle switches
                    switch (row) {
                        case 1: s.darkMode    = !s.darkMode;    break;
                        case 2: s.wifiEnabled = !s.wifiEnabled; break;
                        case 3: s.bleEnabled  = !s.bleEnabled;  break;
                        case 4: s.volume      = (s.volume == 0) ? 60 : 0; break;
                        default: return false;
                    }
                }

                SettingsManager::instance().save();
                _dirty = true;
                return true;
            }
        }
    }
    if (ev == TouchEvent::SwipeDown) {
        AppManager::instance().pop();
        return true;
    }
    return false;
}
