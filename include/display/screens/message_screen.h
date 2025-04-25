#ifndef MESSAGE_SCREEN_H
#define MESSAGE_SCREEN_H

#include "base_screen.h"

// Base class for all message-type screens (error, info, success, warning)
class MessageScreen : public BaseScreen
{
protected:
    char message[128];
    const char *title;
    uint16_t colorTheme;

public:
    MessageScreen(ST7789_AVR &tft, const char *title, uint16_t color);

    void setMessage(const char *msg);
    virtual void draw() override;

    // Each message type implements its own icon
    virtual void drawIcon(int centerX, int centerY) = 0;
    void drawIconCircle(int x, int y, int radius); // Declaration added
};

// Implementation for each message type
class ErrorScreen : public MessageScreen
{
public:
    ErrorScreen(ST7789_AVR &tft);
    void drawIcon(int centerX, int centerY) override;
};

class InfoScreen : public MessageScreen
{
public:
    InfoScreen(ST7789_AVR &tft);
    void drawIcon(int centerX, int centerY) override;
};

class SuccessScreen : public MessageScreen
{
public:
    SuccessScreen(ST7789_AVR &tft);
    void drawIcon(int centerX, int centerY) override;
};

class WarningScreen : public MessageScreen
{
public:
    WarningScreen(ST7789_AVR &tft);
    void drawIcon(int centerX, int centerY) override;
};

#endif // MESSAGE_SCREEN_H