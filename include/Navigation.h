#pragma once
#include "AppManager.h"
#include "EventManager.h"

// Navigation wires EventManager events to AppManager screen transitions.
class Navigation {
public:
    void begin();

private:
    void _onNav(const EventData &d);
};
