#include "stepper.h"
#include "service.h"
#include "sensor.h"
#include "display.h"
#include "log.h"
#include <Wire.h>
int az_offset = 0;
int alt_offset = 0;
double mag_declination = 3.02;
double target_az = 0;
double target_alt = 22.56835;
double target_velocity = 1.0/240.0;
double current_az = 0;
double current_alt = 0;
double current_velocity = 0;
double current_temp = 0.0;
double velocity_diff = 0.0;
void render(void *)
{
    while (1)
    {
        display_clear(true);
        display_draw_cross(64, 32, 127, 64, false);
        display_draw_cross(64 + az_offset, 32 + alt_offset, 10, 10, false);
        char * str = (char*)malloc(16);
        sprintf(str,"AZ:%.0f ALT:%.0f\0",current_az,current_alt);
        display_draw_text(8, 0, str, false);
        sprintf(str,"SPD:%.4f\0",target_velocity);
        display_draw_text(8, 52, str, false);
        free(str);
        display_render();
        delay(1);
    }
}
void setup()
{
    LOG_INIT(115200);
    sensor_init();
    display_init();
    stepper_init();
    stepper_runSpeed(target_velocity);
    service_init();
    LOG_UART("ready!\n");
    xTaskCreatePinnedToCore(render, "render", 8192, NULL, 10, NULL, 1);
    // while(1)
    // {
    //     sensor_calibration();
    // }
}
void loop()
{
    // current_temp = sensor_get_temperation();
    current_az = (sensor_get_azimuth() + mag_declination);// * 0.3 + current_az * 0.7;
    current_alt = sensor_get_pitch() * 0.3 + current_alt * 0.7;
    // current_velocity = sensor_get_velocity();

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
    // velocity_diff = current_velocity - target_velocity;
    // if(velocity_diff<0.5&&velocity_diff>-0.5)
    // stepper_runSpeed((-target_velocity + (velocity_diff * 2.5)));
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