#pragma once
#include <stdint.h>

struct AuraSettings {
    uint8_t  brightness  = 200;
    bool     darkMode    = true;
    uint8_t  volume      = 60;
    bool     wifiEnabled = true;
    bool     bleEnabled  = false;
    char     timezone[32] = "UTC";
};

class SettingsManager {
public:
    static SettingsManager &instance();

    bool begin();
    void save();
    void reset();

    AuraSettings &settings() { return _s; }

private:
    SettingsManager() = default;
    // Preferences included only in .cpp to avoid framework header scan issues
    void        *_prefsHandle = nullptr;  // opaque — cast to Preferences* in .cpp
    AuraSettings _s;
};
