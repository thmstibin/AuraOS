#pragma once
#include <Arduino_GFX_Library.h>
#include "Config.h"
#include "Colors.h"

class StatusBar {
public:
    StatusBar() = default;
    void draw(Arduino_GFX *gfx, uint8_t battPct, bool charging, bool wifi) const;

private:
    void _drawBattery(Arduino_GFX *gfx, int16_t x, int16_t y,
                      uint8_t pct, bool charging) const;
    void _drawWifi(Arduino_GFX *gfx, int16_t x, int16_t y, bool connected) const;
    void _drawTime(Arduino_GFX *gfx) const;
};
