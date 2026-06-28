#pragma once
#include "AppManager.h"
#include "StatusBar.h"

class WiFiApp : public BaseApp {
public:
    void begin(Arduino_GFX *gfx) override;
    void update() override;
    void draw() override;
    bool onTouch(TouchEvent ev, int16_t x, int16_t y) override;

private:
    enum State { SCANNING, NETWORK_LIST, CONNECTING, CONNECTED };

    void _drawScanning() const;
    void _drawNetworkList() const;
    void _drawConnecting() const;
    void _drawConnected() const;

    Arduino_GFX *_gfx = nullptr;
    StatusBar _statusBar;
    bool _dirty = true;
    State _state = SCANNING;
    int _selected = 0;
    uint32_t _lastScan = 0;
};
