#include "DisplayManager.h"
#include "Config.h"

// Driver: CO5300 QSPI AMOLED — Waveshare ESP32-S3-Touch-AMOLED-2.06
// Bus:    Arduino_ESP32QSPI (hardware QSPI, faster than Arduino_QSPI)
// Note:   col_offset1=22 is mandatory for this panel; row/col offset2 = 0

static Arduino_DataBus *_bus;
static Arduino_CO5300  *_panel;

DisplayManager &DisplayManager::instance() {
    static DisplayManager inst;
    return inst;
}

bool DisplayManager::begin() {
    _bus = new Arduino_ESP32QSPI(
        DISP_CS, DISP_SCK, DISP_D0, DISP_D1, DISP_D2, DISP_D3
    );
    // Constructor: (bus, rst, rotation, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
    _panel = new Arduino_CO5300(_bus, DISP_RST, 0,
                                SCREEN_W, SCREEN_H,
                                LCD_COL_OFFSET, 0, 0, 0);

    if (!_panel->begin()) return false;

    _gfx = _panel;
    _w = SCREEN_W;
    _h = SCREEN_H;
    _gfx->fillScreen(0x0000);
    return true;
}

void DisplayManager::setBrightness(uint8_t level) {
    // CO5300 has no Display_Brightness() — send WRDISBV (0x51) via bus.
    // AMOLED defaults to full brightness, so this is non-critical for boot.
    _bus->beginWrite();
    _bus->writeCommand(0x51);
    _bus->write(level);
    _bus->endWrite();
}
