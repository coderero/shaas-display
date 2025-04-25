#include <display/screens/message_screen.h>
#include <string.h>

// Base MessageScreen implementation
MessageScreen::MessageScreen(ST7789_AVR &display, const char *headerTitle, uint16_t color)
    : BaseScreen(display), title(headerTitle), colorTheme(color)
{
    message[0] = '\0';
}

void MessageScreen::setMessage(const char *msg)
{
    strncpy(message, msg, sizeof(message) - 1);
    message[sizeof(message) - 1] = '\0';
}

void MessageScreen::drawIconCircle(int x, int y, int radius)
{
    tft.drawCircle(x, y, radius, colorTheme);
}

void MessageScreen::draw()
{
    // Background panel - make width and height more proportional
    int bgWidth = 220;
    int bgHeight = 220; // Made equal to width for better proportion
    int bgX = (SCREEN_WIDTH - bgWidth) / 2;
    int bgY = (SCREEN_HEIGHT - bgHeight) / 2;

    // Draw panel background
    tft.fillRoundRect(bgX, bgY, bgWidth, bgHeight, 10, COLOR_BG);
    tft.drawRoundRect(bgX, bgY, bgWidth, bgHeight, 10, colorTheme);

    // Draw header
    tft.fillRect(bgX + 10, bgY + 10, bgWidth - 20, 30, colorTheme);
    tft.setTextColor(COLOR_BG);
    tft.setTextSize(2);

    // Center header text
    int titleWidth = strlen(title) * 12; // Approximate width of title text
    int titleX = bgX + (bgWidth - titleWidth) / 2;
    tft.setCursor(titleX, bgY + 18);
    tft.print(title);

    // Draw message with word wrapping
    drawWrappedText(message, bgX + 15, bgY + 60, bgWidth - 40, 20, colorTheme);

    // Draw icon - positioned more centrally
    int iconX = bgX + bgWidth / 2;
    int iconY = bgY + 140; // Moved down slightly for better spacing
    drawIcon(iconX, iconY);
}

// ErrorScreen implementation
ErrorScreen::ErrorScreen(ST7789_AVR &tft)
    : MessageScreen(tft, "ERROR", COLOR_ERROR)
{
    setMessage("System Error: WiFi connection lost");
}

void ErrorScreen::drawIcon(int centerX, int centerY)
{
    drawIconCircle(centerX, centerY, 25);

    // Error icon (exclamation in circle)
    tft.fillRect(centerX - 3, centerY - 15, 6, 20, colorTheme);
    tft.fillRect(centerX - 3, centerY + 8, 6, 6, colorTheme);
}

// InfoScreen implementation
InfoScreen::InfoScreen(ST7789_AVR &tft)
    : MessageScreen(tft, "INFO", COLOR_INFO)
{
    setMessage("System Info: Data uploaded to cloud");
}

void InfoScreen::drawIcon(int centerX, int centerY)
{
    drawIconCircle(centerX, centerY, 25);

    // Info icon (i in circle)
    tft.fillRect(centerX - 3, centerY - 15, 6, 20, colorTheme);
    tft.fillRect(centerX - 3, centerY + 8, 6, 6, colorTheme);
}

// SuccessScreen implementation
SuccessScreen::SuccessScreen(ST7789_AVR &tft)
    : MessageScreen(tft, "SUCCESS", COLOR_SUCCESS)
{
    setMessage("Success: System configuration updated");
}

void SuccessScreen::drawIcon(int centerX, int centerY)
{
    drawIconCircle(centerX, centerY, 25);

    // Success icon (checkmark in circle)
    tft.drawLine(centerX - 15, centerY, centerX - 5, centerY + 10, colorTheme);
    tft.drawLine(centerX - 5, centerY + 10, centerX + 15, centerY - 10, colorTheme);
    tft.drawLine(centerX - 14, centerY + 1, centerX - 5, centerY + 11, colorTheme);
    tft.drawLine(centerX - 5, centerY + 11, centerX + 14, centerY - 9, colorTheme);
}

// WarningScreen implementation
WarningScreen::WarningScreen(ST7789_AVR &tft)
    : MessageScreen(tft, "WARNING", COLOR_WARNING)
{
    setMessage("Warning: Battery level low");
}

void WarningScreen::drawIcon(int centerX, int centerY)
{
    // Warning icon (triangle with exclamation mark)
    for (int i = 0; i < 3; i++)
    {
        tft.drawLine(centerX, centerY - 20 + i, centerX - 20 + i, centerY + 15, colorTheme);
        tft.drawLine(centerX - 20 + i, centerY + 15, centerX + 20 - i, centerY + 15, colorTheme);
        tft.drawLine(centerX + 20 - i, centerY + 15, centerX, centerY - 20 + i, colorTheme);
    }

    // Exclamation mark inside triangle
    tft.fillRect(centerX - 3, centerY - 10, 6, 15, colorTheme);
    tft.fillRect(centerX - 3, centerY + 8, 6, 6, colorTheme);
}