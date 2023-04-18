#include "stepper.h"
#include "service.h"
#include "sensor.h"
#include "display.h"
#include "log.h"
#include <Wire.h>
void setup()
{
    LOG_INIT(115200);
    sensor_init();
    display_init();
    display_clear(false);
    display_render();
    delay(1000);
    display_clear(true);
    display_render();

    // stepper_init();
    service_init();
    LOG_UART("ready!\n");
}
double ang = 0;
void loop()
{
    service_run();
    int x = 64 + 20 * sin(ang);
    int y = 32 + 20 * cos(ang);
    display_clear(true);
    display_draw_cross(64, 32, 127, 64, false);
    display_draw_cross(x, y, 20, 20, false);
    display_render();
    ang += 0.1;
}
/*
    //check i2c address
    int n = 0xa;
    while(1)
    {
      Wire.beginTransmission(n);
      Wire.write(0x80);
      int err = Wire.endTransmission();
      if(err==0)
      {
        LOG_UART("addr:%x\n",n);
      }
      n++;
      if(n>0xf0)
      {
        while(1)delay(100);
      }
      delay(10);
    }
    */