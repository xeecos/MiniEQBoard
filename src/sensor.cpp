#include "sensor.h"
#include "config.h"
#include <QMC5883LCompass.h>
#include <MPU6050.h>
#include "log.h"
#include "TLE5012B.h"
MPU6050 mpu;
QMC5883LCompass compass;
TLE5012B angler;
void sensor_init()
{
    Wire.begin(I2C_SDA, I2C_SCL, 400000);
    compass.init();
    compass.setSmoothing(10, true);
    mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
    mpu.calibrateGyro();
    mpu.setThreshold(3);
    angler.begin(SPI_MISO, SPI_MOSI, SPI_SCK, SPI_CS);
}
double sensor_get_azimuth()
{
    compass.read();
    int az = compass.getAzimuth();
    // LOG_UART("x:%d  y:%d z:%d\n", compass.getX(), compass.getY(), compass.getZ());
    return az;
}
double sensor_get_pitch()
{
    Vector norm = mpu.readScaledAccel();
    double pitch = (1.0 - norm.YAxis) * 90.0;
    return pitch;
}
double sensor_get_velocity()
{
    float speed = 0.0f;
    angler.getAngleSpeed(speed);
    return speed;
}