#include "EventManager.h"

EventManager &EventManager::instance() {
    static EventManager inst;
    return inst;
}

void EventManager::subscribe(Event ev, EventHandler handler) {
    _subs.push_back({ev, handler});
}

void EventManager::emit(const EventData &data) {
    for (auto &s : _subs) {
        if (s.ev == data.type) s.handler(data);
    }
}
