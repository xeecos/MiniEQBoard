#include "display.h"
#include "SSD1315.h"
SSD1315 _oled;
void display_init()
{
    _oled.begin();
}
void display_clear(bool black)
{
    _oled.clearScreen(black);
}
void display_render()
{
    _oled.sendBuffer();
}
void display_fill_rect(int x, int y, int w, int h, bool black)
{
    _oled.fillRect(x, y, w, h, black);
}
void display_draw_line(int x0, int y0, int x1, int y1, bool black)
{
    _oled.drawLine(x0, y0, x1, y1, black);
}
void display_draw_rect(int x, int y, int w, int h, bool black)
{
    _oled.drawRect(x, y, w, h, black);
}
void display_draw_cross(int x, int y, int w, int h, bool black)
{
    _oled.drawCross(x, y, w, h, black);
}
void display_draw_text(int x, int y, const char *str, bool black)
{
    unsigned char i = 0;
    while (str[i])
    {
        unsigned char C = (str[i]);
        for (int ii = 0; ii < 8; ii++)
        {
            unsigned char b = BasicFont[C*8+ii];
            for (int j = 0; j < 8; j++) {
                _oled.setPixel(x + 7 - j, y + ii, ((b >> j) & 1) ? black : !black);
            }
        }
        x+=8;
        i++;
    }
}