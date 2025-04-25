#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

#include <ST7789_AVR.h>
#include "../config.h"

class BaseScreen
{
protected:
    ST7789_AVR &tft;

    // Helper for word wrapping messages
    void drawWrappedText(const char *text, int x, int y, int width, int lineHeight, uint16_t color);

public:
    BaseScreen(ST7789_AVR &tft);
    virtual ~BaseScreen() = default;

    // Every screen must implement this
    virtual void draw() = 0;
};

#endif // BASE_SCREEN_H