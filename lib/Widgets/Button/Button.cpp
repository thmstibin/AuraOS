#include "Button.h"

Button::Button(int16_t x, int16_t y, int16_t w, int16_t h, const char *label)
    : _x(x), _y(y), _w(w), _h(h), _label(label) {}

void Button::draw(Arduino_GFX *gfx, bool pressed) const {
    uint16_t bg = pressed ? Colors::AccentDim : _bg;
    gfx->fillRoundRect(_x, _y, _w, _h, _radius, bg);
    gfx->setTextColor(_fg);
    gfx->setTextSize(2);
    int16_t tx = _x + (_w - strlen(_label) * 12) / 2;
    int16_t ty = _y + (_h - 16) / 2;
    gfx->setCursor(tx, ty);
    gfx->print(_label);
}

bool Button::contains(int16_t px, int16_t py) const {
    return px >= _x && px < _x + _w && py >= _y && py < _y + _h;
}
