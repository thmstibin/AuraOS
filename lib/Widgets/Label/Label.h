#pragma once
#include <Arduino_GFX_Library.h>
#include "Colors.h"

class Label {
public:
    Label() = default;
    Label(int16_t x, int16_t y, const char *text, uint8_t size = 2,
          uint16_t color = Colors::TextPrimary);

    void draw(Arduino_GFX *gfx) const;
    void setText(const char *t) { _text = t; }
    void setColor(uint16_t c)   { _color = c; }

private:
    int16_t     _x = 0, _y = 0;
    const char *_text  = "";
    uint8_t     _size  = 2;
    uint16_t    _color = Colors::TextPrimary;
};
