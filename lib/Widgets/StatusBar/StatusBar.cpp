#include "StatusBar.h"
#include <Arduino.h>
#include <time.h>

void StatusBar::_drawBattery(Arduino_GFX *gfx, int16_t x, int16_t y,
                              uint8_t pct, bool charging) const {
    uint16_t color = (pct > 20) ? Colors::Green :
                     (pct > 10) ? Colors::Yellow : Colors::Red;
    gfx->drawRoundRect(x, y, 22, 11, 2, color);
    gfx->fillRect(x + 22, y + 3, 2, 5, color);
    uint16_t fill = (uint16_t)((pct / 100.0f) * 20);
    gfx->fillRect(x + 1, y + 1, fill, 9, color);
    if (charging) {
        gfx->setTextColor(Colors::White);
        gfx->setTextSize(1);
        gfx->setCursor(x + 6, y + 2);
        gfx->print("+");
    }
}

void StatusBar::_drawWifi(Arduino_GFX *gfx, int16_t x, int16_t y, bool connected) const {
    uint16_t c = connected ? Colors::White : Colors::TextDim;
    gfx->drawCircle(x + 6, y + 12, 4,  c);
    gfx->drawCircle(x + 6, y + 12, 8,  c);
    gfx->drawCircle(x + 6, y + 12, 12, c);
    gfx->fillRect(x, y, 13, 12, Colors::Black);  // clip top arc halves
    gfx->fillCircle(x + 6, y + 12, 2, c);
}

void StatusBar::_drawTime(Arduino_GFX *gfx) const {
    struct tm t;
    if (!getLocalTime(&t)) return;
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", t.tm_hour, t.tm_min);
    gfx->setTextColor(Colors::TextPrimary);
    gfx->setTextSize(1);
    // Center the time in the status bar
    gfx->setCursor(SCREEN_W / 2 - 15, (STATUS_BAR_H - 8) / 2);
    gfx->print(buf);
}

void StatusBar::draw(Arduino_GFX *gfx, uint8_t battPct, bool charging, bool wifi) const {
    gfx->fillRect(0, 0, SCREEN_W, STATUS_BAR_H, Colors::StatusBg);
    _drawTime(gfx);
    _drawWifi(gfx, 8, (STATUS_BAR_H - 14) / 2, wifi);
    _drawBattery(gfx, SCREEN_W - 32, (STATUS_BAR_H - 11) / 2, battPct, charging);
}
