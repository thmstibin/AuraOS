#pragma once
#include <Arduino_GFX_Library.h>
#include "Colors.h"
#include <functional>

class Button {
public:
    Button() = default;
    Button(int16_t x, int16_t y, int16_t w, int16_t h, const char *label);

    void draw(Arduino_GFX *gfx, bool pressed = false) const;
    bool contains(int16_t px, int16_t py) const;

    void setColors(uint16_t bg, uint16_t fg) { _bg = bg; _fg = fg; }
    void setLabel(const char *label)          { _label = label; }

private:
    int16_t     _x = 0, _y = 0, _w = 0, _h = 0;
    const char *_label = "";
    uint16_t    _bg = Colors::Accent;
    uint16_t    _fg = Colors::White;
    uint8_t     _radius = 12;
};
