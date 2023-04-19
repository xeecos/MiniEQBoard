#include "sensor.h"
#include "config.h"
#include <QMC5883LCompass.h>
#include <MPU6050.h>
#include "TLE5012B.h"
MPU6050 mpu;
QMC5883LCompass compass;
TLE5012B angler;
void sensor_init()
{
    Wire.begin(I2C_SDA, I2C_SCL, 400000);
    compass.init();
    mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
    mpu.calibrateGyro();
    mpu.setThreshold(3);
    angler.begin(SPI_MISO, SPI_MOSI, SPI_SCK, SPI_CS);
}
double sensor_get_azimuth()
{
    compass.read();
    int az = compass.getAzimuth();
    if (az > 180)
    {
        az -= 360;
    }
    return az;
}
double sensor_get_pitch()
{
    Vector normAccel = mpu.readNormalizeAccel();
    double pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
    return pitch;
}
double sensor_get_velocity()
{
    float speed = 0.0f;
    angler.getAngleSpeed(speed);
    return speed;
}