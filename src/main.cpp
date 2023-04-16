// // #include "stepper.h"
// // #include "service.h"
// // void setup()
// // {
// //     stepper_init();
// //     service_init();
// // }
// // void loop()
// // {
// //     service_run();
// // }

#include "config.h"

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
void checkSettings();
void setup() 
{
    Wire.setPins(I2C_SDA,I2C_SCL);
  USBSerial.begin(115200);

  // Initialize MPU6050
  USBSerial.println("Initialize MPU6050");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    USBSerial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // If you want, you can set gyroscope offsets
  // mpu.setGyroOffsetX(155);
  // mpu.setGyroOffsetY(15);
  // mpu.setGyroOffsetZ(15);
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);
  
  // Check settings
  checkSettings();
}

void checkSettings()
{
  USBSerial.println();
  
  USBSerial.print(" * Sleep Mode:        ");
  USBSerial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  USBSerial.print(" * Clock Source:      ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     USBSerial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: USBSerial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: USBSerial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      USBSerial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      USBSerial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      USBSerial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  USBSerial.println("Internal 8MHz oscillator"); break;
  }
  
  USBSerial.print(" * Gyroscope:         ");
  switch(mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        USBSerial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        USBSerial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         USBSerial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         USBSerial.println("250 dps"); break;
  } 
  
  USBSerial.print(" * Gyroscope offsets: ");
  USBSerial.print(mpu.getGyroOffsetX());
  USBSerial.print(" / ");
  USBSerial.print(mpu.getGyroOffsetY());
  USBSerial.print(" / ");
  USBSerial.println(mpu.getGyroOffsetZ());
  
  USBSerial.println();
}

void loop()
{
  Vector rawGyro = mpu.readRawGyro();
  Vector normGyro = mpu.readNormalizeGyro();

  USBSerial.print(" Xraw = ");
  USBSerial.print(rawGyro.XAxis);
  USBSerial.print(" Yraw = ");
  USBSerial.print(rawGyro.YAxis);
  USBSerial.print(" Zraw = ");
  USBSerial.println(rawGyro.ZAxis);

  USBSerial.print(" Xnorm = ");
  USBSerial.print(normGyro.XAxis);
  USBSerial.print(" Ynorm = ");
  USBSerial.print(normGyro.YAxis);
  USBSerial.print(" Znorm = ");
  USBSerial.println(normGyro.ZAxis);
  
  delay(10);
}




// #include <QMC5883LCompass.h>
// #include "config.h"
// QMC5883LCompass compass;

// void setup() {
//     Wire.setPins(I2C_SDA,I2C_SCL);
//   USBSerial.begin(115200);
//   compass.init();

// }

// void loop() {
//   int a;

//   // Read compass values
//   compass.read();

//   // Return Azimuth reading
//   a = compass.getAzimuth();

//   USBSerial.print("A: ");
//   USBSerial.print(a);
//   USBSerial.println();

//   delay(250);
// }
