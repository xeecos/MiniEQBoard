#include "sensor.h"
#include "config.h"
#include <QMC5883LCompass.h>
#include <MPU6050.h>

MPU6050 mpu;
QMC5883LCompass compass;
void sensor_init()
{
    Wire.begin(I2C_SDA, I2C_SCL, 400000);
    compass.init();
    mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
    mpu.calibrateGyro();
    mpu.setThreshold(3);
}
double sensor_get_azimuth()
{
    compass.read();
    return compass.getAzimuth();
}
double sensor_get_pitch()
{

    Vector rawGyro = mpu.readRawGyro();
    Vector normGyro = mpu.readNormalizeGyro();
    //   USBSerial.print(" Xraw = ");
    //   USBSerial.print(rawGyro.XAxis);
    //   USBSerial.print(" Yraw = ");
    //   USBSerial.print(rawGyro.YAxis);
    //   USBSerial.print(" Zraw = ");
    //   USBSerial.println(rawGyro.ZAxis);

    //   USBSerial.print(" Xnorm = ");
    //   USBSerial.print(normGyro.XAxis);
    //   USBSerial.print(" Ynorm = ");
    //   USBSerial.print(normGyro.YAxis);
    //   USBSerial.print(" Znorm = ");
    //   USBSerial.println(normGyro.ZAxis);
    return normGyro.YAxis;
}
double sensor_get_velocity()
{
    return 0.0;
}