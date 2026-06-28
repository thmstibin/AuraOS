#pragma once
#include "AppManager.h"
#include "StatusBar.h"
#include "SettingsManager.h"
#include "Button.h"

class SettingsApp : public BaseApp {
public:
    void begin(Arduino_GFX *gfx) override;
    void update()                override;
    void draw()                  override;
    bool onTouch(TouchEvent ev, int16_t x, int16_t y) override;
    void onResume()              override { _dirty = true; }

private:
    void _drawRow(int16_t row, const char *label, const char *value) const;

    Arduino_GFX      *_gfx   = nullptr;
    StatusBar         _statusBar;
    Button            _backBtn;
    bool              _dirty = true;
};
