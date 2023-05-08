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
    // compass.setCalibration(-2491,0,0,5760,-6746,0);
    compass.setCalibration(-1515, 253, -1072, 857, -1483, 0);
    compass.setSmoothing(10, true);
    mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
    mpu.calibrateGyro();
    mpu.setThreshold(3);
    mpu.setI2CMasterModeEnabled(false);
    mpu.setI2CBypassEnabled(true);
    mpu.setSleepEnabled(false);
    // angler.begin(SPI_MISO, SPI_MOSI, SPI_SCK, SPI_CS);
}
double sensor_get_azimuth()
{
    compass.read();
    int az = compass.getAzimuth();
    // int az = atan2(compass.getY(), compass.getZ()) * 180.0 / M_PI;
    // LOG_UART("%d,%d,%d\n", compass.getX(), compass.getY(), compass.getZ());
    return az;
}
double sensor_get_pitch()
{
    Vector norm = mpu.readRawAccel();
    if (norm.ZAxis > 32768)
    {
        norm.ZAxis = 65536 - norm.ZAxis;
    }
    if (norm.XAxis > 32768)
    {
        norm.XAxis = 65536 - norm.XAxis;
    }
    if (norm.YAxis > 32768)
    {
        norm.YAxis = 65536 - norm.YAxis;
    }
    double pitch = 90 - atan2(norm.YAxis, sqrt(norm.ZAxis * norm.ZAxis + norm.XAxis * norm.XAxis)) * 180.0 / M_PI;
    // LOG_UART("x:%.0f  y:%.0f z:%.0f\n", norm.XAxis, norm.YAxis, norm.ZAxis);
    return pitch;
}
double last_angle = 0.0;
long last_time = 0;
double last_speed = 0.0;
double sensor_get_velocity()
{
    if (micros() - last_time > 10000000)
    {
        float angle = 0.0f;
        angler.getAngleValue(angle);
        long time = micros() - last_time;
        if (angle > 180)
        {
            angle -= 360;
        }
        double speed = (angle - last_angle) / (time / 1000000.0);
        last_angle = angle;
        last_time = micros();
        if (speed < 2 && speed > -2)
        {
            last_speed = last_speed * 0.8 + speed * 0.2;
            LOG_UART("%.4f,%.4f,%.4f\n", -1.0 / 240.0, last_speed, speed);
        }
        return last_speed;
    }
    return last_speed;
}
double sensor_get_temperation()
{
    return mpu.readTemperature();
}

#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H
struct Vector
{
    float XAxis;
    float YAxis;
    float ZAxis;
};
#endif

float sensor_tilt()
{
    compass.read();
    Vector mag;
    mag.XAxis = compass.getZ();
    mag.YAxis = compass.getY();
    mag.ZAxis = compass.getX();
    // Vector normAccel = mpu.readScaledAccel();

    // Pitch & Roll

    Vector norm = mpu.readRawAccel();
    if (norm.ZAxis > 32768)
    {
        norm.ZAxis = norm.ZAxis - 65536;
    }
    if (norm.XAxis > 32768)
    {
        norm.XAxis = norm.XAxis - 65536;
    }
    if (norm.YAxis > 32768)
    {
        norm.YAxis = norm.YAxis - 65536;
    }
    double pitch = atan2(norm.YAxis, norm.ZAxis);//sqrt(norm.ZAxis * norm.ZAxis + norm.XAxis * norm.XAxis));
    double roll = atan2(norm.XAxis, sqrt(norm.ZAxis * norm.ZAxis + norm.YAxis * norm.YAxis));

    // float v = sqrt((norm.XAxis * norm.XAxis) + (norm.YAxis * norm.YAxis) + (norm.ZAxis * norm.ZAxis)); 
    // float roll = asin(norm.XAxis / v);
    // float pitch = asin(norm.YAxis / v);


    // pitch = asin(-normAccel.XAxis);

    // if (roll > 0.78 || roll < -0.78 || pitch > 0.78 || pitch < -0.78)
    // {
    //     return sensor_get_azimuth();
    // }

    // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
    float cosRoll = cos(roll);
    float sinRoll = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);

    // Tilt compensation
    // float Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
    // float Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;
    float Xh = mag.XAxis * cosPitch + mag.YAxis * sinRoll * sinPitch - mag.ZAxis * cosRoll * sinPitch;
    float Yh = mag.YAxis * cosRoll + mag.ZAxis * sinRoll;
    float heading = atan2(Yh, Xh) * 180.0 / M_PI;
    printf("heading:%.1f, %.1f %.1f\n", heading, pitch * 180.0 / M_PI,roll * 180.0 / M_PI);
    return heading;
}
int calibrationData[3][2];
bool changed = false;
bool done = false;
int t = 0;
int c = 0;
void sensor_calibration()
{
    int x, y, z;
    // Read compass values
    compass.read();

    // Return XYZ readings
    x = compass.getX();
    y = compass.getY();
    z = compass.getZ();

    changed = false;

    if (x < calibrationData[0][0])
    {
        calibrationData[0][0] = x;
        changed = true;
    }
    if (x > calibrationData[0][1])
    {
        calibrationData[0][1] = x;
        changed = true;
    }

    if (y < calibrationData[1][0])
    {
        calibrationData[1][0] = y;
        changed = true;
    }
    if (y > calibrationData[1][1])
    {
        calibrationData[1][1] = y;
        changed = true;
    }

    if (z < calibrationData[2][0])
    {
        calibrationData[2][0] = z;
        changed = true;
    }
    if (z > calibrationData[2][1])
    {
        calibrationData[2][1] = z;
        changed = true;
    }

    if (changed && !done)
    {
        LOG_UART("CALIBRATING... Keep moving your sensor around.\n");
        c = millis();
    }
    t = millis();

    if ((t - c > 5000) && !done)
    {
        done = true;
        LOG_UART("DONE. Copy the line below and paste it into your projects sketch.);\n");
        LOG_UART("compass.setCalibration(%d,%d,%d,%d,%d,%d);\n", calibrationData[0][0], calibrationData[0][1], calibrationData[1][0], calibrationData[1][1], calibrationData[2][0], calibrationData[2][1]);
    }
}