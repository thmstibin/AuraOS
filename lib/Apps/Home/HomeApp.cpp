#include "HomeApp.h"
#include "PowerManager.h"
#include "Colors.h"
#include "Config.h"
#include <time.h>
#include <Arduino.h>

void HomeApp::begin(Arduino_GFX *gfx) {
    _gfx   = gfx;
    _dirty = true;
}

void HomeApp::onResume() {
    _dirty = true;
}

void HomeApp::update() {
    struct tm t;
    if (getLocalTime(&t) && t.tm_min != _lastMin) {
        _lastMin = t.tm_min;
        _dirty   = true;
    }
}

void HomeApp::_drawClock() {
    struct tm t;
    char buf[6] = "--:--";
    if (getLocalTime(&t)) snprintf(buf, sizeof(buf), "%02d:%02d", t.tm_hour, t.tm_min);

    // Large time display — centered
    _gfx->setTextColor(Colors::White);
    _gfx->setTextSize(6);
    int16_t tw = strlen(buf) * 36;
    _gfx->setCursor((SCREEN_W - tw) / 2, SCREEN_H / 2 - 60);
    _gfx->print(buf);

    // Seconds
    if (getLocalTime(&t)) {
        char sec[3];
        snprintf(sec, sizeof(sec), "%02d", t.tm_sec);
        _gfx->setTextSize(2);
        _gfx->setTextColor(Colors::TextSecond);
        _gfx->setCursor((SCREEN_W + tw) / 2 + 4, SCREEN_H / 2 - 36);
        _gfx->print(sec);
    }
}

void HomeApp::_drawDate() {
    struct tm t;
    if (!getLocalTime(&t)) return;
    char buf[24];
    // e.g. "Saturday, Jun 28"
    strftime(buf, sizeof(buf), "%A, %b %d", &t);
    _gfx->setTextColor(Colors::TextSecond);
    _gfx->setTextSize(1);
    int16_t tw = strlen(buf) * 6;
    _gfx->setCursor((SCREEN_W - tw) / 2, SCREEN_H / 2 + 10);
    _gfx->print(buf);
}

void HomeApp::draw() {
    if (!_dirty) return;
    _dirty = false;

    auto &pm = PowerManager::instance();

    // Background
    _gfx->fillRect(0, STATUS_BAR_H, SCREEN_W, SCREEN_H - STATUS_BAR_H - NAV_BAR_H,
                   Colors::Black);
    _statusBar.draw(_gfx, pm.batteryPercent(), pm.isCharging(), false);
    _navBar.draw(_gfx, NavTab::Home);
    _drawClock();
    _drawDate();

    // Swipe hint
    _gfx->setTextColor(Colors::TextDim);
    _gfx->setTextSize(1);
    _gfx->setCursor(SCREEN_W / 2 - 36, SCREEN_H - NAV_BAR_H - 20);
    _gfx->print("swipe up");
}

bool HomeApp::onTouch(TouchEvent ev, int16_t x, int16_t y) {
    // Tap to refresh display
    if (ev == TouchEvent::Tap) {
        _dirty = true;
        Serial.printf("[Home] Tap at x=%d y=%d\n", x, y);
        return true;
    }
    // Swipe feedback (v0.2: will open launcher/navigate)
    if (ev == TouchEvent::SwipeUp || ev == TouchEvent::SwipeDown ||
        ev == TouchEvent::SwipeLeft || ev == TouchEvent::SwipeRight) {
        _dirty = true;
        return true;
    }
    return false;
}
