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