#pragma once
#include <stdint.h>

class PowerManager {
public:
    static PowerManager &instance();

    bool begin();
    void update();

    uint8_t batteryPercent() const { return _pct; }
    bool    isCharging()     const { return _charging; }
    bool    isCritical()     const { return _pct < 10; }

private:
    PowerManager() = default;
    uint8_t _pct     = 100;
    bool    _charging = false;
};
