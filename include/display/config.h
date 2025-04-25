#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Display pins
#define TFT_DC 8  // Data/Command pin
#define TFT_RST 9 // Reset pin
#define TFT_CS -1 // Chip select pin

// Screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

// Screen duration
#define SCREEN_DURATION 5000 // 5 seconds per screen

// Color definitions
#define COLOR_BG 0x0000        // Black background
#define COLOR_TEXT 0xFFFF      // White text
#define COLOR_ERROR 0xF8C6     // Rose red for errors
#define COLOR_INFO 0x0C9F      // Royal blue for info
#define COLOR_SUCCESS 0x07E0   // Emerald for success
#define COLOR_WARNING 0xFFE0   // Yellow for warnings
#define COLOR_HIGHLIGHT 0x841F // Purple for highlights

// Display modes
enum DisplayMode
{
    MODE_ERROR,
    MODE_INFO,
    MODE_SUCCESS,
    MODE_WARNING,
    MODE_SENSORS,
    MODE_DEVICES,
    MODE_COUNT
};

#endif // CONFIG_H