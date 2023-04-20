#include "stepper.h"
#include "service.h"
#include "sensor.h"
#include "display.h"
#include "log.h"
#include <Wire.h>
int az_offset = 0;
int alt_offset = 0;
double temp = 0.0;
void render(void *)
{
    while (1)
    {
        display_clear(true);
        display_draw_cross(64, 32, 127, 64, false);
        display_draw_cross(64 + az_offset, 32 + alt_offset, 10, 10, false);
        char * str = (char*)malloc(16);
        sprintf(str,"az:%d alt:%d\0",az_offset,alt_offset);
        display_draw_text(0, 56, str, false);
        free(str);
        display_render();
        delay(10);
    }
}
void setup()
{
    LOG_INIT(115200);
    sensor_init();
    display_init();
    stepper_init();
    service_init();
    LOG_UART("ready!\n");
    xTaskCreatePinnedToCore(render, "render", 8192, NULL, 10, NULL, 1);
    // while(1)
    // {
    //     sensor_calibration();
    // }
}
double mag_declination = 3.02;
double target_az = 0;
double target_alt = 22.56835;
double target_velocity = 0.004;
double current_az = 0;
double current_alt = 0;
double current_velocity = 0;
void loop()
{
    temp = sensor_get_temperation();
    current_az = (sensor_get_azimuth() + mag_declination) * 0.2 + current_az * 0.8;
    current_alt = sensor_get_pitch() * 0.2 + current_alt * 0.8;
    current_velocity = sensor_get_velocity();

    az_offset = (current_az - target_az) * 5;
    if (az_offset < -50)
    {
        az_offset = -50;
    }
    else if (az_offset > 50)
    {
        az_offset = 50;
    }
    alt_offset = (current_alt - target_alt) * 5;
    if (alt_offset < -25)
    {
        alt_offset = -25;
    }
    else if (alt_offset > 25)
    {
        alt_offset = 25;
    }
    double velocity_diff = current_velocity - target_velocity;
    stepper_runSpeed((target_velocity - (velocity_diff * 0.1)));
    service_run();
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