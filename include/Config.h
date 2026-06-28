#pragma once

// ─── AuraOS Hardware Config ───────────────────────────────────────────────────
// Board: Waveshare ESP32-S3-Touch-AMOLED-2.06
// Pins sourced from Waveshare official pin_config.h
// Repo: github.com/waveshareteam/ESP32-S3-Touch-AMOLED-2.06

// ─── Display (CO5300 QSPI AMOLED 2.06", 410×502) ────────────────────────────
#define SCREEN_W        410
#define SCREEN_H        502
#define LCD_SDIO0       4
#define LCD_SDIO1       5
#define LCD_SDIO2       6
#define LCD_SDIO3       7
#define LCD_SCLK        11
#define LCD_CS          12
#define LCD_RESET       8
// col_offset1=22 is required for CO5300 on this board — do not change
#define LCD_COL_OFFSET  22

// Convenience aliases used by DisplayManager
#define DISP_CS         LCD_CS
#define DISP_SCK        LCD_SCLK
#define DISP_D0         LCD_SDIO0
#define DISP_D1         LCD_SDIO1
#define DISP_D2         LCD_SDIO2
#define DISP_D3         LCD_SDIO3
#define DISP_RST        LCD_RESET

// ─── Touch (FT3168, I2C @ 0x38) ──────────────────────────────────────────────
#define TOUCH_I2C_ADDR  0x38
#define TOUCH_SDA       15
#define TOUCH_SCL       14
#define TOUCH_INT       38
#define TOUCH_RST       9

// ─── Power Management (AXP2101, shared I2C bus) ───────────────────────────────
// AXP2101 sits on the same I2C bus as touch (SDA=15, SCL=14)
// Display power rails are enabled by AXP2101 defaults on this board.
// If screen stays black: add XPowersLib and call axp.enableDC1() / axp.enableALDO3().
#define AXP2101_I2C_ADDR  0x34

// ─── SD Card ──────────────────────────────────────────────────────────────────
#define SDMMC_CLK       2
#define SDMMC_CMD       1
#define SDMMC_DATA      3
#define SDMMC_CS        17

// ─── Audio (ES8311 codec, shared I2C + I2S) ───────────────────────────────────
#define AUDIO_I2C_ADDR  0x18
#define AUDIO_I2S_BCLK  39
#define AUDIO_I2S_WS    40
#define AUDIO_I2S_DOUT  41
#define AUDIO_I2S_DIN   42

// ─── Power ────────────────────────────────────────────────────────────────────
#define BAT_ADC_PIN     -1   // Battery % read via AXP2101, not raw ADC
#define CHARGE_PIN      -1

// ─── UI ───────────────────────────────────────────────────────────────────────
#define STATUS_BAR_H    32
#define NAV_BAR_H       56
#define TOUCH_DEAD_ZONE 4    // min px movement to register swipe
