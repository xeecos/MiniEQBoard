#include "stepper.h"
#include "service.h"
#include "sensor.h"
#include "display.h"
#include "log.h"
#include <Wire.h>

double target_velocity = 1.0/240.0;

double target_alt = 28.226499;
double current_alt = 0;
double alt_offset = 0;

void render(void *)
{
    while (1)
    {
        current_alt = sensor_get_pitch() * 0.3 + current_alt * 0.7;
        alt_offset = (current_alt - target_alt) * 5;
        if (alt_offset < -25)
        {
            alt_offset = -25;
        }
        else if (alt_offset > 25)
        {
            alt_offset = 25;
        }
        display_clear(true);
        display_draw_cross(64, 32, 127, 64, false);
        display_draw_cross(64, 32 + alt_offset, 10, 10, false);
        char * str = (char*)malloc(16);
        sprintf(str,"ALT:%.0f\0",current_alt);
        display_draw_text(8, 0, str, false);
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
    xTaskCreatePinnedToCore(render, "render", 8192, NULL, 10, NULL, 1);
    stepper_init();
    // stepper_runSpeed(target_velocity);
    // service_init();
    LOG_UART("ready!\n");
}
void loop()
{
    service_run();
}