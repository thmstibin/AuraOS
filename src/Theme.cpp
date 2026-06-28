#include "Theme.h"

ThemeManager &ThemeManager::instance() {
    static ThemeManager inst;
    return inst;
}
