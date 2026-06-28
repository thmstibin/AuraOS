#pragma once
#include <Arduino_GFX_Library.h>

class DisplayManager {
public:
    static DisplayManager &instance();

    bool begin();
    void setBrightness(uint8_t level);  // 0–255

    Arduino_GFX *gfx() { return _gfx; }
    uint16_t width()  const { return _w; }
    uint16_t height() const { return _h; }

private:
    DisplayManager() = default;
    Arduino_GFX *_gfx = nullptr;
    uint16_t _w = 0, _h = 0;
};
