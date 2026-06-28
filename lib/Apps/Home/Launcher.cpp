#include "Launcher.h"
#include "Icons.h"
#include "Colors.h"
#include "Config.h"
#include "HomeApp.h"
#include "SettingsApp.h"
#include "AppManager.h"
#include "PowerManager.h"
#include <Arduino.h>

// Simple placeholder app for unimplemented features
class PlaceholderApp : public BaseApp {
public:
    PlaceholderApp(const char *name) : _name(name) {}

    void begin(Arduino_GFX *gfx) override {
        _gfx = gfx;
        _dirty = true;
    }
    void update() override {}
    void draw() override {
        if (!_dirty) return;
        _dirty = false;

        _gfx->fillScreen(Colors::Black);

        // Title
        _gfx->setTextColor(Colors::Accent);
        _gfx->setTextSize(4);
        int16_t tw = strlen(_name) * 24;
        _gfx->setCursor((SCREEN_W - tw) / 2, 80);
        _gfx->print(_name);

        // Coming soon message
        _gfx->setTextSize(2);
        _gfx->setTextColor(Colors::TextSecond);
        _gfx->setCursor(40, 200);
        _gfx->print("Coming soon");

        _gfx->setTextSize(1);
        _gfx->setTextColor(Colors::TextDim);
        _gfx->setCursor(30, 260);
        _gfx->print("in v0.3+");

        // Back hint
        _gfx->setTextColor(Colors::TextDim);
        _gfx->setCursor(20, SCREEN_H - 40);
        _gfx->print("Swipe down to go back");
    }
    bool onTouch(TouchEvent ev, int16_t x, int16_t y) override {
        if (ev == TouchEvent::SwipeDown) {
            AppManager::instance().pop();
            return true;
        }
        return false;
    }

private:
    Arduino_GFX *_gfx = nullptr;
    bool _dirty = false;
    const char *_name;
};

static constexpr AppEntry APPS[9] = {
    {"Home",     Icons::home},
    {"Settings", Icons::settings},
    {"WiFi",     Icons::wifi},
    {"Clock",    Icons::clock},
    {"Battery",  Icons::home},
    {"Info",     Icons::apps},
    {"App 7",    Icons::home},
    {"App 8",    Icons::clock},
    {"App 9",    Icons::wifi},
};

void Launcher::begin(Arduino_GFX *gfx) {
    _gfx = gfx;
    _dirty = true;
    Serial.println("[Launcher] Started");
}

void Launcher::_drawApp(int col, int row, int idx, bool highlighted) const {
    const AppEntry &app = APPS[idx];
    // Center grid on screen: 3 cells * 95px = 285px wide, centered in 410px screen
    int gridWidth = 3 * (CELL - 10);  // 285px
    int startX = (SCREEN_W - gridWidth) / 2;  // center horizontally
    int startY = STATUS_BAR_H + 20;  // top margin below status bar

    int x = startX + col * CELL;
    int y = startY + row * CELL;

    uint16_t bgColor = highlighted ? Colors::Overlay : Colors::Card;
    uint16_t textColor = highlighted ? Colors::Accent : Colors::White;

    // Cell background
    _gfx->fillRect(x, y, CELL - 10, CELL - 10, bgColor);

    // Cell border
    _gfx->drawRect(x, y, CELL - 10, CELL - 10, textColor);

    // Icon (centered, 48x48)
    app.iconFn(_gfx, x + (CELL - 58) / 2, y + 10, textColor);

    // Label
    _gfx->setTextColor(textColor);
    _gfx->setTextSize(1);
    int16_t tw = strlen(app.name) * 6;
    _gfx->setCursor(x + (CELL - 58) / 2 + (48 - tw) / 2, y + 65);
    _gfx->print(app.name);
}

void Launcher::draw() {
    if (!_dirty) return;
    _dirty = false;

    // Background
    _gfx->fillRect(0, STATUS_BAR_H, SCREEN_W, SCREEN_H - STATUS_BAR_H - NAV_BAR_H,
                   Colors::Black);

    // Draw 3x3 grid
    for (int i = 0; i < 9; i++) {
        int col = i % 3;
        int row = i / 3;
        _drawApp(col, row, i, i == _highlight);
    }

    // Status bar
    PowerManager &pm = PowerManager::instance();
    _statusBar.draw(_gfx, pm.batteryPercent(), pm.isCharging(), false);
}

bool Launcher::onTouch(TouchEvent ev, int16_t x, int16_t y) {
    // Swipe down to close
    if (ev == TouchEvent::SwipeDown) {
        AppManager::instance().pop();
        return true;
    }

    // Tap to launch app
    if (ev == TouchEvent::Tap) {
        int gridWidth = 3 * (CELL - 10);
        int startX = (SCREEN_W - gridWidth) / 2;
        int startY = STATUS_BAR_H + 20;

        if (x >= startX && y >= startY) {
            int col = (x - startX) / CELL;
            int row = (y - startY) / CELL;

            if (col >= 0 && col < 3 && row >= 0 && row < 3) {
                int idx = row * 3 + col;
                _launchApp(idx);
                return true;
            }
        }
    }

    return false;
}

void Launcher::_launchApp(int idx) {
    Serial.printf("[Launcher] Launching app %d (%s)\n", idx, APPS[idx].name);

    switch (idx) {
        case 0:  // Home
            while (!AppManager::instance().empty()) {
                AppManager::instance().pop();
            }
            AppManager::instance().push(new HomeApp());
            break;

        case 1:  // Settings
            AppManager::instance().push(new SettingsApp());
            break;

        default:  // All others are placeholders
            AppManager::instance().push(new PlaceholderApp(APPS[idx].name));
            break;
    }
}
