#pragma once
#include "AppManager.h"
#include "StatusBar.h"
#include "NavigationBar.h"

class HomeApp : public BaseApp {
public:
    void begin(Arduino_GFX *gfx) override;
    void update()                override;
    void draw()                  override;
    bool onTouch(TouchEvent ev, int16_t x, int16_t y) override;
    void onResume()              override;

private:
    void _drawClock();
    void _drawDate();

    Arduino_GFX    *_gfx     = nullptr;
    StatusBar       _statusBar;
    NavigationBar   _navBar;
    bool            _dirty   = true;
    uint8_t         _lastMin = 255;
};
