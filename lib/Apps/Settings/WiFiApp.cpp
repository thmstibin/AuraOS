#include "WiFiApp.h"
#include "WiFiManager.h"
#include "Colors.h"
#include "Config.h"
#include <Arduino.h>

void WiFiApp::begin(Arduino_GFX *gfx) {
    _gfx = gfx;
    _dirty = true;
    _state = SCANNING;
    Serial.println("[WiFiApp] Scanning networks...");
    WiFiManager::instance().scan();
    _lastScan = millis();
}

void WiFiApp::update() {
    WiFiManager::instance().update();
    if (WiFiManager::instance().isConnected()) {
        _state = CONNECTED;
        _dirty = true;
    }
}

void WiFiApp::_drawScanning() const {
    _gfx->fillScreen(Colors::Black);
    _gfx->setTextColor(Colors::White);
    _gfx->setTextSize(3);
    _gfx->setCursor(50, 150);
    _gfx->print("WiFi");

    _gfx->setTextSize(2);
    _gfx->setTextColor(Colors::TextSecond);
    _gfx->setCursor(40, 250);
    _gfx->print("Scanning...");
}

void WiFiApp::_drawNetworkList() const {
    _gfx->fillScreen(Colors::Black);

    // Title
    _gfx->setTextColor(Colors::White);
    _gfx->setTextSize(2);
    _gfx->setCursor(100, 40);
    _gfx->print("Networks");

    auto &wm = WiFiManager::instance();
    int count = wm.networkCount();

    if (count == 0) {
        _gfx->setTextSize(1);
        _gfx->setTextColor(Colors::TextSecond);
        _gfx->setCursor(30, 150);
        _gfx->print("No networks found");
        _gfx->setCursor(30, 170);
        _gfx->print("Swipe down to rescan");
        return;
    }

    // Network list (show up to 5)
    for (int i = 0; i < count && i < 5; i++) {
        const auto *net = wm.getNetwork(i);
        if (!net) continue;

        int y = 100 + i * 40;
        uint16_t color = (i == _selected) ? Colors::Accent : Colors::White;

        if (i == _selected) {
            _gfx->fillRect(10, y - 5, SCREEN_W - 20, 35, Colors::Card);
        }

        _gfx->setTextColor(color);
        _gfx->setTextSize(1);
        _gfx->setCursor(20, y);
        _gfx->print(net->ssid);

        // Signal strength indicator
        int bars = 0;
        if (net->rssi > -50) bars = 4;
        else if (net->rssi > -70) bars = 3;
        else if (net->rssi > -85) bars = 2;
        else bars = 1;

        _gfx->setCursor(SCREEN_W - 40, y);
        _gfx->print("[");
        for (int b = 0; b < bars; b++) _gfx->print("|");
        _gfx->print("]");
    }

    _gfx->setTextSize(1);
    _gfx->setTextColor(Colors::TextDim);
    _gfx->setCursor(20, SCREEN_H - 40);
    _gfx->print("Tap to connect, Swipe down to close");
}

void WiFiApp::_drawConnecting() const {
    _gfx->fillScreen(Colors::Black);
    _gfx->setTextColor(Colors::Accent);
    _gfx->setTextSize(2);
    _gfx->setCursor(80, 150);
    _gfx->print("Connecting");

    _gfx->setTextColor(Colors::TextSecond);
    _gfx->setTextSize(1);
    _gfx->setCursor(120, 200);
    _gfx->print("...");
}

void WiFiApp::_drawConnected() const {
    _gfx->fillScreen(Colors::Black);
    auto &wm = WiFiManager::instance();

    _gfx->setTextColor(Colors::Green);
    _gfx->setTextSize(3);
    _gfx->setCursor(100, 100);
    _gfx->print("WiFi");

    _gfx->setTextColor(Colors::White);
    _gfx->setTextSize(2);
    _gfx->setCursor(50, 180);
    _gfx->print("Connected!");

    _gfx->setTextColor(Colors::TextSecond);
    _gfx->setTextSize(1);
    _gfx->setCursor(30, 240);
    _gfx->print("Network: ");
    _gfx->print(wm.connectedSSID());

    _gfx->setCursor(30, 260);
    _gfx->printf("Signal: %d dBm", wm.signalStrength());

    _gfx->setTextColor(Colors::TextDim);
    _gfx->setCursor(20, SCREEN_H - 40);
    _gfx->print("Swipe down to close");
}

void WiFiApp::draw() {
    if (!_dirty) return;
    _dirty = false;

    switch (_state) {
        case SCANNING:
            _drawScanning();
            if (WiFiManager::instance().networkCount() > 0) {
                _state = NETWORK_LIST;
                _dirty = true;
            }
            break;
        case NETWORK_LIST:
            _drawNetworkList();
            break;
        case CONNECTING:
            _drawConnecting();
            break;
        case CONNECTED:
            _drawConnected();
            break;
    }
}

bool WiFiApp::onTouch(TouchEvent ev, int16_t x, int16_t y) {
    auto &wm = WiFiManager::instance();

    if (ev == TouchEvent::SwipeDown) {
        AppManager::instance().pop();
        return true;
    }

    if (_state == NETWORK_LIST) {
        if (ev == TouchEvent::SwipeUp) {
            _selected = (_selected + 1) % wm.networkCount();
            _dirty = true;
            return true;
        }
        if (ev == TouchEvent::Tap) {
            const auto *net = wm.getNetwork(_selected);
            if (net) {
                // TODO: prompt for password, for now use empty
                Serial.printf("[WiFiApp] Connecting to %s\n", net->ssid);
                if (wm.connect(net->ssid, "")) {
                    _state = CONNECTED;
                } else {
                    _state = NETWORK_LIST;
                }
                _dirty = true;
                return true;
            }
        }
    }

    return false;
}
