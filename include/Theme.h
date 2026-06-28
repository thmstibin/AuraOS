#pragma once
#include "Colors.h"

struct Theme {
    uint16_t background;
    uint16_t surface;
    uint16_t card;
    uint16_t textPrimary;
    uint16_t textSecondary;
    uint16_t accent;
};

class ThemeManager {
public:
    static ThemeManager &instance();
    const Theme &current() const { return _dark; }

private:
    ThemeManager() = default;
    Theme _dark {
        Colors::Black,
        Colors::Surface,
        Colors::Card,
        Colors::TextPrimary,
        Colors::TextSecond,
        Colors::Accent,
    };
};
