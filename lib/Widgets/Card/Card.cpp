#include "Card.h"

Card::Card(int16_t x, int16_t y, int16_t w, int16_t h)
    : _x(x), _y(y), _w(w), _h(h) {}

void Card::draw(Arduino_GFX *gfx) const {
    gfx->fillRoundRect(_x, _y, _w, _h, _radius, Colors::Card);
    gfx->drawRoundRect(_x, _y, _w, _h, _radius, Colors::Overlay);
}

void Card::drawWithTitle(Arduino_GFX *gfx, const char *title) const {
    draw(gfx);
    gfx->setTextColor(Colors::TextPrimary);
    gfx->setTextSize(2);
    gfx->setCursor(_x + 12, _y + 12);
    gfx->print(title);
}
