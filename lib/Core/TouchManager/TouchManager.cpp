#include "TouchManager.h"
#include "Config.h"
#include <Wire.h>
#include <Arduino.h>

// FT3168 (FocalTech) register map — read 7 bytes from 0x00:
// [0x00] DevMode  [0x01] Gesture  [0x02] TD_STATUS (touch count)
// [0x03] TOUCH1_XH  [0x04] TOUCH1_XL
// [0x05] TOUCH1_YH  [0x06] TOUCH1_YL
// X = ((XH & 0x0F) << 8) | XL,  Y = ((YH & 0x0F) << 8) | YL

#define SWIPE_THRESHOLD 40   // px — increased slightly for reliability
#define LONG_PRESS_MS   500

TouchManager &TouchManager::instance() {
    static TouchManager inst;
    return inst;
}

bool TouchManager::begin() {
    Serial.println("[Touch] Initializing...");
    Wire.begin(TOUCH_SDA, TOUCH_SCL);
    Wire.setClock(400000);
    Serial.println("[Touch] I2C initialized");

    if (TOUCH_RST >= 0) {
        Serial.println("[Touch] Resetting FT3168...");
        pinMode(TOUCH_RST, OUTPUT);
        digitalWrite(TOUCH_RST, LOW);
        delay(10);
        digitalWrite(TOUCH_RST, HIGH);
        delay(100);
        Serial.println("[Touch] Reset complete");
    }
    if (TOUCH_INT >= 0) {
        pinMode(TOUCH_INT, INPUT);
    }

    Serial.println("[Touch] Scanning for FT3168...");
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    uint8_t err = Wire.endTransmission();
    if (err == 0) {
        Serial.println("[Touch] FT3168 FOUND!");
    } else {
        Serial.printf("[Touch] FT3168 not found (err=%d) - scanning...\n", err);
        for (uint8_t addr = 1; addr < 127; addr++) {
            Wire.beginTransmission(addr);
            if (Wire.endTransmission() == 0) {
                Serial.printf("[Touch] Device at 0x%02X\n", addr);
            }
        }
    }
    return true;
}

void TouchManager::_read() {
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(0x00);
    if (Wire.endTransmission(false) != 0) {
        _last.pressed = false;
        return;
    }
    Wire.requestFrom((uint8_t)TOUCH_I2C_ADDR, (uint8_t)7);
    if (Wire.available() < 7) {
        _last.pressed = false;
        return;
    }
    Wire.read();                      // 0x00 DevMode
    Wire.read();                      // 0x01 Gesture
    uint8_t fingers = Wire.read();    // 0x02 TD_STATUS
    uint8_t xh = Wire.read();         // 0x03 XH
    uint8_t xl = Wire.read();         // 0x04 XL
    uint8_t yh = Wire.read();         // 0x05 YH
    uint8_t yl = Wire.read();         // 0x06 YL

    _last.pressed = ((fingers & 0x0F) > 0);
    if (_last.pressed) {
        _last.x = ((xh & 0x0F) << 8) | xl;
        _last.y = ((yh & 0x0F) << 8) | yl;
    }
}

void TouchManager::_classify(int16_t dx, int16_t dy, uint32_t ms) {
    if (!_cb) return;
    int16_t ax = abs(dx), ay = abs(dy);

    Serial.printf("[Touch] classify dx=%d dy=%d ms=%u\n", dx, dy, ms);

    if (ax < TOUCH_DEAD_ZONE && ay < TOUCH_DEAD_ZONE) {
        TouchEvent ev = (ms >= LONG_PRESS_MS) ? TouchEvent::LongPress : TouchEvent::Tap;
        Serial.println(ev == TouchEvent::Tap ? "[Touch] -> Tap" : "[Touch] -> LongPress");
        _cb(ev, _startX, _startY);
        return;
    }
    if (ax > ay) {
        bool left = (dx < 0);
        Serial.printf("[Touch] -> %s\n", left ? "SwipeLeft" : "SwipeRight");
        _cb(left ? TouchEvent::SwipeLeft : TouchEvent::SwipeRight, _startX, _startY);
    } else {
        // dy < 0  →  finger moved toward top of screen  →  SwipeUp
        // dy > 0  →  finger moved toward bottom          →  SwipeDown
        // FT3168 Y=0 is at top (same as display), so this is correct.
        // If swipes feel inverted, flip the comparison here:
        //   bool up = (dy > 0);   // uncomment this line and comment the next
        bool up = (dy < 0);
        Serial.printf("[Touch] -> %s (dy=%d)\n", up ? "SwipeUp" : "SwipeDown", dy);
        _cb(up ? TouchEvent::SwipeUp : TouchEvent::SwipeDown, _startX, _startY);
    }
}

void TouchManager::update() {
    _read();
    uint32_t now = millis();

    if (_last.pressed && !_tracking) {
        _tracking = true;
        _startX   = _last.x;
        _startY   = _last.y;
        _startMs  = now;
        _endX = _last.x;
        _endY = _last.y;
        Serial.printf("[Touch] down x=%d y=%d\n", _last.x, _last.y);
    } else if (_last.pressed && _tracking) {
        _endX = _last.x;
        _endY = _last.y;
    } else if (!_last.pressed && _tracking) {
        _tracking = false;
        int16_t dx = _endX - _startX;
        int16_t dy = _endY - _startY;
        Serial.printf("[Touch] up dx=%d dy=%d\n", dx, dy);
        _classify(dx, dy, now - _startMs);
    }
}
