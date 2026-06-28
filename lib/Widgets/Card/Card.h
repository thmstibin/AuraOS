#pragma once
#include <Arduino_GFX_Library.h>
#include "Colors.h"

class Card {
public:
    Card() = default;
    Card(int16_t x, int16_t y, int16_t w, int16_t h);

    void draw(Arduino_GFX *gfx) const;
    void drawWithTitle(Arduino_GFX *gfx, const char *title) const;

    int16_t contentY() const { return _y + 8; }
    int16_t contentX() const { return _x + 12; }
    int16_t contentW() const { return _w - 24; }

private:
    int16_t  _x = 0, _y = 0, _w = 0, _h = 0;
    uint8_t  _radius = 14;
};
