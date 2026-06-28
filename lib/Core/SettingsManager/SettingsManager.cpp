#include "SettingsManager.h"
#include <Preferences.h>
#include <string.h>

static Preferences _prefs;

SettingsManager &SettingsManager::instance() {
    static SettingsManager inst;
    return inst;
}

bool SettingsManager::begin() {
    _prefs.begin("auraos", false);
    _s.brightness  = _prefs.getUChar("bright",  200);
    _s.darkMode    = _prefs.getBool("dark",     true);
    _s.volume      = _prefs.getUChar("vol",     60);
    _s.wifiEnabled = _prefs.getBool("wifi",     true);
    _s.bleEnabled  = _prefs.getBool("ble",      false);
    String tz      = _prefs.getString("tz",     "UTC");
    strncpy(_s.timezone, tz.c_str(), sizeof(_s.timezone) - 1);
    return true;
}

void SettingsManager::save() {
    _prefs.putUChar("bright", _s.brightness);
    _prefs.putBool("dark",    _s.darkMode);
    _prefs.putUChar("vol",    _s.volume);
    _prefs.putBool("wifi",    _s.wifiEnabled);
    _prefs.putBool("ble",     _s.bleEnabled);
    _prefs.putString("tz",    _s.timezone);
}

void SettingsManager::reset() {
    _prefs.clear();
    _s = AuraSettings{};
}
