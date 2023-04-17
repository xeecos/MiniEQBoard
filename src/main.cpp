#include "stepper.h"
#include "service.h"
#include "sensor.h"
void setup()
{
    stepper_init();
    sensor_init();
    service_init();
}
void loop()
{
    service_run();
}