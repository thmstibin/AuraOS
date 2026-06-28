#pragma once
#include "AppManager.h"
#include "StatusBar.h"

struct AppEntry {
    const char *name;
    void (*iconFn)(Arduino_GFX *, int16_t, int16_t, uint16_t);
};

class Launcher : public BaseApp {
public:
    void begin(Arduino_GFX *gfx) override;
    void update()                override {}
    void draw()                  override;
    bool onTouch(TouchEvent ev, int16_t x, int16_t y) override;
    void onResume()              override { _dirty = true; }

private:
    void _drawApp(int col, int row, int idx, bool highlighted) const;
    void _launchApp(int idx);

    Arduino_GFX *_gfx      = nullptr;
    StatusBar    _statusBar;
    bool         _dirty    = true;
    int          _highlight = -1;

    static constexpr int COLS = 3;
    static constexpr int ROWS = 3;
    static constexpr int CELL = 100;
};
