#include "Boot.h"
#include "Colors.h"
#include "Config.h"
#include "Version.h"
#include <Arduino.h>

static const int16_t LOGO_R  = 48;
static const int16_t BAR_W   = 200;
static const int16_t BAR_H   = 6;
static const int16_t BAR_Y   = SCREEN_H * 3 / 4;

void Boot::_logo(Arduino_GFX *gfx) {
    int16_t cx = SCREEN_W / 2, cy = SCREEN_H / 3;

    // Outer ring
    gfx->drawCircle(cx, cy, LOGO_R,     Colors::Accent);
    gfx->drawCircle(cx, cy, LOGO_R - 2, Colors::Accent);

    // "A" glyph inside circle
    gfx->setTextColor(Colors::White);
    gfx->setTextSize(5);
    gfx->setCursor(cx - 15, cy - 20);
    gfx->print("A");

    // Product name
    gfx->setTextSize(2);
    gfx->setTextColor(Colors::White);
    gfx->setCursor(cx - 34, cy + LOGO_R + 16);
    gfx->print("AuraOS");

    // Version
    gfx->setTextSize(1);
    gfx->setTextColor(Colors::TextSecond);
    gfx->setCursor(cx - 14, cy + LOGO_R + 38);
    gfx->print(AURA_VERSION_STR);
}

void Boot::_progress(Arduino_GFX *gfx, uint8_t pct) {
    int16_t bx = (SCREEN_W - BAR_W) / 2;
    gfx->drawRoundRect(bx, BAR_Y, BAR_W, BAR_H, 3, Colors::Overlay);
    int16_t fill = (int16_t)(pct / 100.0f * (BAR_W - 2));
    gfx->fillRoundRect(bx + 1, BAR_Y + 1, fill, BAR_H - 2, 2, Colors::Accent);
}

void Boot::run(Arduino_GFX *gfx) {
    gfx->fillScreen(Colors::Black);
    _logo(gfx);

    // Animate progress bar while "booting"
    for (uint8_t p = 0; p <= 100; p += 2) {
        _progress(gfx, p);
        delay(15);
    }
    delay(300);
}
