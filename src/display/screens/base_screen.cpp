#include <display/screens/base_screen.h>
#include <string.h>

BaseScreen::BaseScreen(ST7789_AVR &display) : tft(display)
{
}

void BaseScreen::drawWrappedText(const char *text, int x, int y, int width, int lineHeight, uint16_t color)
{
    // Set text properties
    tft.setTextColor(color);
    tft.setTextSize(1);

    // Calculate chars per line (approximately)
    const int charsPerLine = width / 6; // Assuming 6 pixels per character

    // Get text length
    int len = strlen(text);

    if (len <= charsPerLine)
    {
        // If text fits on a single line, center it
        tft.setCursor(x + (width - len * 6) / 2, y);
        tft.print(text);
    }
    else
    {
        // Split into multiple lines
        char buffer[128];
        strncpy(buffer, text, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        char *ptr = buffer;
        int lineCount = 0;

        while (*ptr && lineCount < 4)
        { // Max 4 lines
            // Calculate end of current line
            int lineLen = min(strlen(ptr), (size_t)charsPerLine);

            // Try to break at a space for better word wrapping
            if (lineLen < strlen(ptr))
            {
                // Look for a space to break at
                for (int i = lineLen; i > 0; i--)
                {
                    if (ptr[i] == ' ')
                    {
                        lineLen = i;
                        break;
                    }
                }
            }

            // Temporarily null-terminate this line
            char next = ptr[lineLen];
            ptr[lineLen] = '\0';

            // Display the line
            tft.setCursor(x, y + lineCount * lineHeight);
            tft.print(ptr);

            // Restore the character and move to next line
            ptr[lineLen] = next;
            ptr += lineLen;

            // Skip any spaces at the start of next line
            while (*ptr == ' ')
                ptr++;

            lineCount++;
        }
    }
}