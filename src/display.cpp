#include "display.h"
#include "SSD1315.h"
SSD1315 _oled;
void display_init()
{
    _oled.begin();
}

void display_clear()
{
    _oled.clearScreen();
}