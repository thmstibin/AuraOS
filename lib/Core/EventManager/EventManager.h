#pragma once
#include <stdint.h>
#include <functional>
#include <vector>

enum class Event : uint8_t {
    // Navigation
    NavHome,
    NavBack,
    NavLauncher,
    NavApp,
    // System
    BatteryUpdate,
    TimeUpdate,
    WifiStatusChange,
    // App
    AppLaunch,
};

struct EventData {
    Event    type;
    int32_t  i0 = 0;
    int32_t  i1 = 0;
    void    *ptr = nullptr;
};

using EventHandler = std::function<void(const EventData &)>;

class EventManager {
public:
    static EventManager &instance();

    void subscribe(Event ev, EventHandler handler);
    void emit(const EventData &data);
    void emit(Event ev) { emit({ev}); }

private:
    EventManager() = default;
    struct Subscription { Event ev; EventHandler handler; };
    std::vector<Subscription> _subs;
};
