#pragma once
#include <stdint.h>
#include <Arduino_GFX_Library.h>
#include "TouchManager.h"

// Base class all apps/screens must implement
class BaseApp {
public:
    virtual ~BaseApp() = default;
    virtual void begin(Arduino_GFX *gfx) = 0;
    virtual void update()                = 0;
    virtual void draw()                  = 0;
    virtual bool onTouch(TouchEvent ev, int16_t x, int16_t y) = 0;
    // Return true if the event was consumed (stops propagation)
    virtual void onResume() {}  // called when this app becomes active again
};

class AppManager {
public:
    static AppManager &instance();

    void push(BaseApp *app);   // navigate into app (takes ownership)
    void pop();                // go back to previous app
    void replace(BaseApp *app);// swap current screen

    void update();
    void draw();
    void dispatchTouch(TouchEvent ev, int16_t x, int16_t y);

    bool empty() const { return _depth == 0; }

private:
    AppManager() = default;
    static constexpr int STACK_DEPTH = 8;
    BaseApp *_stack[STACK_DEPTH] = {};
    int      _depth = 0;
    Arduino_GFX *_gfx = nullptr;

    friend class AuraOS;
    void setGfx(Arduino_GFX *g) { _gfx = g; }
};
