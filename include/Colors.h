#pragma once
#include <stdint.h>

// RGB565 palette — AMOLED black is true power-saving black
namespace Colors {
    // Backgrounds
    constexpr uint16_t Black       = 0x0000;
    constexpr uint16_t Surface     = 0x1082;  // #111111
    constexpr uint16_t Card        = 0x18C3;  // #181818
    constexpr uint16_t Overlay     = 0x2104;  // #202020

    // Text
    constexpr uint16_t White       = 0xFFFF;
    constexpr uint16_t TextPrimary = 0xFFFF;
    constexpr uint16_t TextSecond  = 0x8410;  // #808080
    constexpr uint16_t TextDim     = 0x4208;  // #404040

    // Accent
    constexpr uint16_t Accent      = 0x051F;  // #0066FF blue
    constexpr uint16_t AccentDim   = 0x020F;  // #003380
    constexpr uint16_t Green       = 0x07E0;
    constexpr uint16_t Red         = 0xF800;
    constexpr uint16_t Yellow      = 0xFFE0;
    constexpr uint16_t Orange      = 0xFD20;

    // Status bar
    constexpr uint16_t StatusBg    = 0x0000;
    constexpr uint16_t BattGood    = 0x07E0;
    constexpr uint16_t BattLow     = 0xFD20;
    constexpr uint16_t BattCrit    = 0xF800;
}
