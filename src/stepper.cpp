#include "stepper.h"
#include "config.h"
#include "TMCStepper.h"
#include "log.h"

HardwareSerial uart(1);
TMC2208Stepper stepper(&uart, 0.2);
hw_timer_t *timer;
STEPPER_MODE stepper_mode = SPEED_MODE;
double current_speed = 0.0;
double target_speed = 0.0;
double current_time;
double angle_per = 1800.0 / MICROSTEPS * 1000.0 / GEAR_RATIO;
double acceleration = 1;
double gear_ratio = GEAR_RATIO;
double acc_speed = sqrt(2.0 * acceleration * angle_per) / 1000.0;
long start_position = 0;
long current_position = 0;
long end_position = 0;
long accelerate_dist;
long target_position = 0;
void stepper_run()
{
    if (stepper_mode == SPEED_MODE)
    {
        double next_speed = current_speed;
        if (current_speed < target_speed)
        {
            next_speed = current_speed + acc_speed;
            if (next_speed > target_speed)
            {
                next_speed = target_speed;
            }
        }
        else if (current_speed > target_speed)
        {
            next_speed = current_speed - acc_speed;
            if (next_speed < target_speed)
            {
                next_speed = target_speed;
            }
        }
        double speed = next_speed > 0 ? next_speed : -next_speed;
        stepper_step(next_speed < 0);
        current_time = angle_per / speed;
        current_speed = next_speed;
        if (current_time < 30)
        {
            current_time = 30;
        }
        timerAlarmWrite(timer, (int)current_time, true);
    }
    else if (stepper_mode == DISTANCE_MODE)
    {
        double next_speed = current_speed;
        int current_dist = start_position - current_position;
        current_dist = current_dist < 0 ? -current_dist : current_dist;
        int rest_dist = end_position - current_position;
        rest_dist = rest_dist < 0 ? -rest_dist : rest_dist;
        if (rest_dist != 0)
        {
            if (current_dist < accelerate_dist)
            {
                next_speed = current_speed + acc_speed;
            }
            else if (rest_dist < accelerate_dist)
            {
                next_speed = current_speed - acc_speed;
            }
            else
            {
            }
            if (next_speed > 1.5)
            {
                next_speed = 1.5;
            }
            if (next_speed < -1.5)
            {
                next_speed = -1.5;
            }
            double speed = next_speed > 0 ? next_speed : -next_speed;
            if (speed == 0)
            {
                speed = 0.001;
                next_speed = 0.001;
            }
            stepper_step((target_position - current_position) > 0);
            current_time = angle_per / speed;
            current_speed = next_speed;
            if (current_time < 25)
            {
                current_time = 25;
            }
            timerAlarmWrite(timer, (int)current_time, true);
        }
    }
    // stepper_step(0);
    // 1000/((3600/3600/24)/(1.8/256/34))
    // delayMicroseconds(40);
    // delay(49);
    // delayMicroseconds(632);
}
void stepper_init()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, HIGH);
    uart.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);
    stepper.begin();
    while (stepper.test_connection())
    {
        LOG_UART("no RA STEPPER\n");
        delay(1000);
    }
    LOG_UART("STEPPER INIT!\n");
    stepper.internal_Rsense(false);
    stepper.dedge(true);
    stepper.microsteps(MICROSTEPS);
    stepper.rms_current(750);
    stepper.toff(2);
    stepper.push();
    digitalWrite(EN_PIN, LOW);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &stepper_run, true);
    timerAlarmWrite(timer, 10000, true);
    timerAlarmEnable(timer);
}
void stepper_microsteps(int microsteps)
{
    stepper.microsteps(microsteps);
    stepper.push();
    angle_per = 1800.0 / stepper.microsteps() * 1000.0 / gear_ratio;
    acc_speed = sqrt(2.0 * acceleration * angle_per) / 1000.0;
}
void stepper_current(int current)
{
    stepper.rms_current(current);
    stepper.push();
}
void stepper_ratio(double ratio)
{
    gear_ratio = ratio;
    angle_per = 1800.0 / stepper.microsteps() * 1000.0 / ratio;
    acc_speed = sqrt(2.0 * acceleration * angle_per) / 1000.0;
}
void stepper_reset()
{
}
void stepper_runSpeed(double speed)
{
    target_speed = speed;
    stepper_mode = SPEED_MODE;
}
void stepper_move(long distance)
{
    stepper_moveTo(current_position + distance);
}
void stepper_moveTo(long position)
{
    target_position = position;
    accelerate_dist = (target_position - current_position) / 4;
    accelerate_dist = accelerate_dist < 0 ? -accelerate_dist : accelerate_dist;
    stepper_mode = DISTANCE_MODE;
}
bool _step = false;
void stepper_step(bool dir)
{
    stepper_direction(dir);
    digitalWrite(STEP_PIN, _step);
    _step = !_step;
}
void stepper_direction(bool dir)
{
    digitalWrite(DIR_PIN, dir);
}