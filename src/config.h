#pragma once
#define BAUD_RATE               115200

#define RX1_PIN                      3
#define TX1_PIN                      4
#define STEP_PIN                     7
#define DIR_PIN                      6
#define EN_PIN                      10
#define LED_PIN                      9
#define I2C_SCL                      0
#define I2C_SDA                      1
#define SPI_MOSI                    20
#define SPI_MISO                     5
#define SPI_SCK                     21
#define SPI_CS                       9
#define QMC5883_ADDR
#define MPU6050_ADDR
#define SSD1315_ADDR

#define MICROSTEPS               256.0
#define GEAR_RATIO                  34

enum STEPPER_MODE
{
    SPEED_MODE = 1,
    DISTANCE_MODE,
};