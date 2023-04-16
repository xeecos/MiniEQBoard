#pragma once
void stepper_init();
void stepper_step(bool dir);
void stepper_direction(bool dir);
void stepper_microsteps(int microsteps);
void stepper_ratio(double ratio);
void stepper_current(int current);
void stepper_run();
void stepper_reset();
void stepper_runSpeed(double speed);
void stepper_move(long distance);
void stepper_moveTo(long position);