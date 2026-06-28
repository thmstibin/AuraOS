#include "Navigation.h"
#include "HomeApp.h"
#include "Launcher.h"
#include "SettingsApp.h"

void Navigation::begin() {
    auto &em = EventManager::instance();
    auto &am = AppManager::instance();

    em.subscribe(Event::NavHome, [&am](const EventData &) {
        while (am.empty() == false) am.pop();
        am.push(new HomeApp());
    });

    em.subscribe(Event::NavLauncher, [&am](const EventData &) {
        am.push(new Launcher());
    });

    em.subscribe(Event::NavBack, [&am](const EventData &) {
        am.pop();
    });

    em.subscribe(Event::NavApp, [&am](const EventData &d) {
        if (d.ptr && strcmp((const char *)d.ptr, "Settings") == 0) {
            am.push(new SettingsApp());
        }
    });
}
