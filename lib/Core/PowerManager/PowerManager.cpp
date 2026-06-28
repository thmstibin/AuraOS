#include "PowerManager.h"
#include "Config.h"
#include <Arduino.h>

PowerManager &PowerManager::instance() {
    static PowerManager inst;
    return inst;
}

bool PowerManager::begin() {
    if (BAT_ADC_PIN >= 0) {
        pinMode(BAT_ADC_PIN, INPUT);
    }
    if (CHARGE_PIN >= 0) {
        pinMode(CHARGE_PIN, INPUT_PULLUP);
    }
    return true;
}

void PowerManager::update() {
    if (BAT_ADC_PIN >= 0) {
        // 12-bit ADC, 3.3 V ref, voltage divider typically halves bat voltage
        uint32_t raw = analogRead(BAT_ADC_PIN);
        float vbat = (raw / 4095.0f) * 3.3f * 2.0f;  // ×2 for divider
        // Li-Po: 4.2 V = 100%, 3.3 V = 0%
        float pct = (vbat - 3.3f) / (4.2f - 3.3f) * 100.0f;
        _pct = (uint8_t)constrain((int)pct, 0, 100);
    }
    if (CHARGE_PIN >= 0) {
        _charging = (digitalRead(CHARGE_PIN) == LOW);
    }
}
