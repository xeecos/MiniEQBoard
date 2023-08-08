#include "stepper.h"
#include "service.h"
#include "sensor.h"
#include "display.h"
#include "log.h"
#include <Wire.h>

double target_velocity = 1.0/240.0;

void setup()
{
    LOG_INIT(115200);
    sensor_init();
    display_init();
    stepper_init();
    stepper_runSpeed(target_velocity);
    service_init();
    LOG_UART("ready!\n");
}
void loop()
{
    service_run();
}