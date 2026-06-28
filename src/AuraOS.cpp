#include "AuraOS.h"
#include "Boot.h"
#include "Navigation.h"
#include "HomeApp.h"
#include "DisplayManager.h"
#include "TouchManager.h"
#include "EventManager.h"
#include "AppManager.h"
#include "PowerManager.h"
#include "SettingsManager.h"
#include "WiFiManager.h"
#include <Arduino.h>
#include <time.h>
#include "Version.h"

static Navigation _nav;

AuraOS &AuraOS::instance() {
    static AuraOS inst;
    return inst;
}

void AuraOS::begin() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n[AuraOS] Starting boot sequence...");

    // 1. Load settings early (brightness needed before display)
    Serial.println("[AuraOS] 1. Initializing Settings...");
    SettingsManager::instance().begin();
    Serial.println("[AuraOS] 1. Settings OK");

    // 2. Display - must be first hardware init
    Serial.println("[AuraOS] 2. Initializing Display...");
    auto &dm = DisplayManager::instance();
    if (!dm.begin()) {
        Serial.println("[AuraOS] Display init failed");
        return;
    }
    Serial.println("[AuraOS] 2. Display OK");
    DisplayManager::instance().setBrightness(SettingsManager::instance().settings().brightness);

    // 3. Touch - init before boot so gestures work immediately after logo
    Serial.println("[AuraOS] 3. Initializing Touch...");
    if (!TouchManager::instance().begin()) {
        Serial.println("[AuraOS] Touch init failed - continuing");
    }
    Serial.println("[AuraOS] 3. Touch OK");

    // 4. AppManager wired to display
    Serial.println("[AuraOS] 4. Wiring AppManager...");
    AppManager::instance().setGfx(dm.gfx());
    Serial.println("[AuraOS] 4. AppManager OK");

    // 5. Boot animation (all hardware ready)
    Serial.println("[AuraOS] 5. Running boot animation...");
    Boot boot;
    boot.run(dm.gfx());
    Serial.println("[AuraOS] 5. Boot animation complete");

    // 6. Remaining subsystems
    Serial.println("[AuraOS] 6. PowerManager...");
    PowerManager::instance().begin();

    // Initialize WiFi (will try to auto-connect to saved network)
    Serial.println("[AuraOS] 6b. WiFi...");
    WiFiManager::instance().begin();

    // Set fallback time (Jan 1, 2025 00:00:00 UTC)
    // Once WiFi connects, NTP will update this to actual time
    time_t epoch = 1735689600;
    struct timeval tv = {epoch, 0};
    settimeofday(&tv, nullptr);
    Serial.println("[AuraOS] 6. Time set (fallback)");

    // 7. Navigation & touch wiring (skip Navigation for v0.1, wire directly)
    Serial.println("[AuraOS] 7. Touch routing...");
    TouchManager::instance().onTouch([](TouchEvent ev, int16_t x, int16_t y) {
        AppManager::instance().dispatchTouch(ev, x, y);
    });
    Serial.println("[AuraOS] 7. Touch routing OK");

    // 9. Push home screen - OS is live
    Serial.println("[AuraOS] 9. Pushing HomeApp...");
    AppManager::instance().push(new HomeApp());
    Serial.println("[AuraOS] 9. HomeApp OK");

    // PSRAM sanity check
    size_t psram = ESP.getPsramSize();
    Serial.printf("[AuraOS] PSRAM: %u bytes (%s)\n", psram,
                  psram >= (8 * 1024 * 1024) ? "OK" : "WARNING - too small");

    _booted = true;
    Serial.println("[AuraOS] Boot complete - " AURA_VERSION_STR);
}

void AuraOS::loop() {
    if (!_booted) return;

    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 5000) {
        lastPrint = millis();
        Serial.println("[AuraOS] Loop running...");
    }

    TouchManager::instance().update();
    PowerManager::instance().update();
    WiFiManager::instance().update();  // Check WiFi connection state
    AppManager::instance().update();
    AppManager::instance().draw();

    delay(16);  // ~60 fps cap
}
