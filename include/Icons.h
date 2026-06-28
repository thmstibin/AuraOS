#pragma once
#include <Arduino_GFX_Library.h>

// Minimal icon drawing helpers using Arduino_GFX primitives.
// Each icon fits a 48×48 cell. Pass the top-left corner (x, y) and GFX pointer.

namespace Icons {

inline void clock(Arduino_GFX *gfx, int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 24, cy = y + 24, r = 22;
    gfx->drawCircle(cx, cy, r, color);
    gfx->drawLine(cx, cy, cx, cy - 14, color);       // hour hand
    gfx->drawLine(cx, cy, cx + 10, cy, color);       // minute hand
    gfx->fillCircle(cx, cy, 2, color);
}

inline void settings(Arduino_GFX *gfx, int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 24, cy = y + 24;
    gfx->drawCircle(cx, cy, 8, color);
    for (int i = 0; i < 8; i++) {
        float a = i * 3.14159f / 4.0f;
        int16_t ox = (int16_t)(cos(a) * 18), oy = (int16_t)(sin(a) * 18);
        gfx->fillCircle(cx + ox, cy + oy, 3, color);
    }
}

inline void home(Arduino_GFX *gfx, int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 24;
    gfx->drawLine(cx, (int16_t)(y+8),  (int16_t)(x+4),  (int16_t)(y+26), color);
    gfx->drawLine(cx, (int16_t)(y+8),  (int16_t)(x+44), (int16_t)(y+26), color);
    gfx->drawRect((int16_t)(x+10), (int16_t)(y+26), 28, 18, color);
    gfx->drawRect((int16_t)(x+18), (int16_t)(y+32), 12, 12, color);
}

inline void apps(Arduino_GFX *gfx, int16_t x, int16_t y, uint16_t color) {
    // 3x3 dot grid
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            gfx->fillCircle(x + 12 + col * 12, y + 12 + row * 12, 3, color);
}

inline void wifi(Arduino_GFX *gfx, int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 24, cy = y + 30;
    gfx->drawCircle(cx, cy, 6,  color);
    gfx->drawCircle(cx, cy, 14, color);
    gfx->drawCircle(cx, cy, 22, color);
    gfx->fillRect(x, y, 48, 30, 0x0000);  // clip top half
    gfx->fillCircle(cx, cy, 3, color);
}

inline void battery(Arduino_GFX *gfx, int16_t x, int16_t y, uint16_t color, uint8_t pct) {
    gfx->drawRect(x, y+2, 20, 10, color);
    gfx->fillRect(x+20, y+4, 2, 6, color);  // nub
    uint16_t fill = (uint16_t)(pct / 100.0f * 18);
    gfx->fillRect(x+1, y+3, fill, 8, color);
}

} // namespace Icons
