#pragma once

class AuraOS {
public:
    static AuraOS &instance();

    void begin();
    void loop();

private:
    AuraOS() = default;
    bool _booted = false;
};
