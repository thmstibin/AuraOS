#include "NavigationBar.h"
#include "Icons.h"

static const int16_t NAV_Y = SCREEN_H - NAV_BAR_H;
static const int16_t TAB_W = SCREEN_W / 3;

void NavigationBar::draw(Arduino_GFX *gfx, NavTab active) const {
    gfx->fillRect(0, NAV_Y, SCREEN_W, NAV_BAR_H, Colors::Surface);
    gfx->drawFastHLine(0, NAV_Y, SCREEN_W, Colors::Overlay);

    const struct { NavTab tab; const char *label; } tabs[3] = {
        { NavTab::Home,     "Home"     },
        { NavTab::Apps,     "Apps"     },
        { NavTab::Settings, "Settings" },
    };

    for (int i = 0; i < 3; i++) {
        bool sel    = (tabs[i].tab == active);
        uint16_t c  = sel ? Colors::Accent : Colors::TextSecond;
        int16_t  cx = TAB_W * i + TAB_W / 2;

        if (i == 0) Icons::home(gfx, cx - 12, NAV_Y + 4, c);
        else if (i == 1) Icons::apps(gfx, cx - 12, NAV_Y + 4, c);
        else             Icons::settings(gfx, cx - 12, NAV_Y + 4, c);

        gfx->setTextColor(c);
        gfx->setTextSize(1);
        int16_t lx = cx - (int16_t)(strlen(tabs[i].label) * 6 / 2);
        gfx->setCursor(lx, NAV_Y + 40);
        gfx->print(tabs[i].label);

        if (sel) gfx->drawFastHLine(TAB_W * i + 8, NAV_Y, TAB_W - 16, Colors::Accent);
    }
}

NavTab NavigationBar::hitTest(int16_t x, int16_t y) const {
    if (!inBounds(x, y)) return NavTab::Home;
    if (x < TAB_W)     return NavTab::Home;
    if (x < TAB_W * 2) return NavTab::Apps;
    return NavTab::Settings;
}

bool NavigationBar::inBounds(int16_t x, int16_t y) const {
    return y >= NAV_Y && y < SCREEN_H && x >= 0 && x < SCREEN_W;
}
