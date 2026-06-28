#include "Label.h"

Label::Label(int16_t x, int16_t y, const char *text, uint8_t size, uint16_t color)
    : _x(x), _y(y), _text(text), _size(size), _color(color) {}

void Label::draw(Arduino_GFX *gfx) const {
    gfx->setTextColor(_color);
    gfx->setTextSize(_size);
    gfx->setCursor(_x, _y);
    gfx->print(_text);
}
