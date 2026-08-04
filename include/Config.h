#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//====================================================
// Firmware
//====================================================

#define FW_NAME        "ClockNano"
#define FW_VERSION     "2.0.0"

//====================================================
// Display
//====================================================

#define DISPLAY_MODULES    4

#define PIN_MAX_DIN        11
#define PIN_MAX_CLK        13
#define PIN_MAX_CS         10

// Hardware FC16
#define DISPLAY_HW MD_MAX72XX::FC16_HW

//====================================================
// RTC
//====================================================

#define PIN_RTC_SDA        A4
#define PIN_RTC_SCL        A5

//====================================================
// Buttons
//====================================================

#define PIN_BTN_MINUS      2
#define PIN_BTN_OK         3
#define PIN_BTN_PLUS       4

//====================================================
// Buzzer
//====================================================

#define PIN_BUZZER         5

#endif