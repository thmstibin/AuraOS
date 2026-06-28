#include "AppManager.h"
#include <Arduino.h>

AppManager &AppManager::instance() {
    static AppManager inst;
    return inst;
}

void AppManager::push(BaseApp *app) {
    if (_depth >= STACK_DEPTH) { delete app; return; }
    _stack[_depth++] = app;
    app->begin(_gfx);
}

void AppManager::pop() {
    if (_depth == 0) return;
    delete _stack[--_depth];
    _stack[_depth] = nullptr;
    if (_depth > 0) _stack[_depth - 1]->onResume();
}

void AppManager::replace(BaseApp *app) {
    if (_depth > 0) { delete _stack[_depth - 1]; _depth--; }
    push(app);
}

void AppManager::update() {
    if (_depth > 0) _stack[_depth - 1]->update();
}

void AppManager::draw() {
    if (_depth > 0) _stack[_depth - 1]->draw();
}

void AppManager::dispatchTouch(TouchEvent ev, int16_t x, int16_t y) {
    if (_depth > 0) _stack[_depth - 1]->onTouch(ev, x, y);
}
