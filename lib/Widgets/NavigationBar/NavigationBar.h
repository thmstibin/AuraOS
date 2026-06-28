#pragma once
#include <Arduino_GFX_Library.h>
#include "Config.h"
#include "Colors.h"

enum class NavTab : uint8_t { Home, Apps, Settings };

class NavigationBar {
public:
    NavigationBar() = default;
    void draw(Arduino_GFX *gfx, NavTab active) const;
    NavTab hitTest(int16_t x, int16_t y) const;
    bool   inBounds(int16_t x, int16_t y) const;
};
