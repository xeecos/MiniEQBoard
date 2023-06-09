
#include "SSD1315.h"
#include <math.h>
#include <Arduino.h>
SSD1315::SSD1315()
{
    _wire = &Wire;
    _i2cAddress = SSD1315_ADDRESS;
    _width = SSD1315_DISPLAY_WIDTH;
    _height = SSD1315_DISPLAY_HEIGHT;
    _buffer = (uint8_t *)malloc(SSD1315_DISPLAY_WIDTH * SSD1315_DISPLAY_HEIGHT / 8);
}

void SSD1315::sendCommand(uint8_t command)
{
    _wire->beginTransmission(_i2cAddress);
    _wire->write(0x80);
    _wire->write(command);
    _wire->endTransmission();
}

void SSD1315::begin()
{
    sendCommand(SSD1315_DISPLAY_OFF);
    sendCommand(SSD1315_SET_DISPLAY_CLOCK);
    sendCommand(0x80);
    sendCommand(SSD1315_SET_MULTIPLEX_RATIO);
    sendCommand(0x3F);
    sendCommand(SSD1315_SET_DISPLAY_OFFSET);
    sendCommand(0x00);
    sendCommand(SSD1315_SET_START_LINE | 0);
    sendCommand(SSD1315_CHARGE_DCDC_PUMP);
    sendCommand(0x14);
    sendCommand(SSD1315_ADDR_MODE);
    sendCommand(0x00);
    sendCommand(SSD1315_SET_REMAP_L_TO_R);
    sendCommand(SSD1315_SET_REMAP_T_TO_D);
    sendCommand(SSD1315_SET_COM_PINS);
    sendCommand(0x12);
    sendCommand(SSD1315_SET_CONTRAST);
    sendCommand(0xFF);
    sendCommand(SSD1315_SET_PRECHARGE_PERIOD);
    sendCommand(0xF1);
    sendCommand(SSD1315_SET_VCOM_DESELECT);
    sendCommand(0x40);
    sendCommand(SSD1315_RAM_ON);
    sendCommand(SSD1315_INVERT_OFF);
    sendCommand(SSD1315_DISPLAY_ON);
}
void SSD1315::sendBuffer()
{
    sendCommand(SSD1315_ADDR_PAGE);
    sendCommand(0);
    sendCommand(_height / 8 - 1);
    sendCommand(SSD1315_ADDR_COLUMN);
    sendCommand(0);
    sendCommand(_width - 1);
    for (uint16_t i = 0, count = _width * _height / 8; i < count; i++)
    {
        _wire->beginTransmission(_i2cAddress);
        _wire->write(0x40);
        _wire->write(_buffer[i]);
        _wire->endTransmission();
    }
}

void SSD1315::clearScreen(bool black)
{
    memset(_buffer, black ? 0x0 : 0xff, _width * _height / 8);
}

void SSD1315::setPixel(int x, int y, bool black)
{
    x = 127 - x;
    y = 63 - y;
    int n = x + int(y / 8) * _width;
    if (black)
    {
        _buffer[n] &= ~(1 << (y % 8));
    }
    else
    {
        _buffer[n] |= (1 << (y % 8));
    }
}
bool SSD1315::getPixel(int x, int y)
{
    int idx = y * 128 + x;
    int n = (idx / 8);
    return (_buffer[n] >> (idx % 8)) & 1;
}
void SSD1315::drawLine(int x0, int y0, int x1, int y1, bool black)
{
    double dx = x1 - x0, dy = y1 - y0;
    int dist = dx * dx + dy * dy;
    int steps = sqrt(dist) + 1;
    dx /= steps;
    dy /= steps;
    double x = x0, y = y0;
    for (int i = 0; i < steps; i++)
    {
        setPixel(x, y, black);
        x += dx;
        y += dy;
    }
}
void SSD1315::drawRect(int x, int y, int w, int h, bool black)
{
    drawLine(x - w / 2 - 1, y - h / 2 - 1, x + w / 2, y - h / 2 - 1, black);
    drawLine(x + w / 2, y - h / 2 - 1, x + w / 2, y + h / 2, black);
    drawLine(x + w / 2, y + h / 2, x - w / 2 - 1, y + h / 2, black);
    drawLine(x - w / 2 - 1, y + h / 2, x - w / 2 - 1, y - h / 2 - 1, black);
}

void SSD1315::drawCross(int x, int y, int w, int h, bool black)
{
    drawLine(x, y - h / 2, x, y + h / 2 + 1, black);
    drawLine(x - w / 2, y, x + w / 2 + 1, y, black);
}

void SSD1315::fillRect(int x, int y, int w, int h, bool black)
{
    for (int i = 0; i < h; i++)
    {
        drawLine(x, y + i, x + w - 1, y + i, black);
    }
}