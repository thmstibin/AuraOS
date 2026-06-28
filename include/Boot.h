#pragma once
#include <Arduino_GFX_Library.h>

class Boot {
public:
    void run(Arduino_GFX *gfx);  // blocking; returns when boot is complete

private:
    void _logo(Arduino_GFX *gfx);
    void _progress(Arduino_GFX *gfx, uint8_t pct);
};
