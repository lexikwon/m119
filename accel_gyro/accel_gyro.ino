/*
  Accelerometer and Gyroscope Reader

  Logs and displays all IMU data

  Adapted from SimpleGyroscope and SimpleAccelerometer examples on IMU

  by Alexandra Kwon (UID: 205290932) for ECE M119
*/

//Include the library for accessing the data
#include <Arduino_LSM6DS3.h>

void setup() {
  //Initialize IMU
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  //Print basic information on IMU
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in g's (AX, AY, AZ) and Gyroscope in degrees/second (GX, GY, GZ)");
  Serial.println("AX\tAY\tAZ\tGX\tGY\tGZ");

}

void loop() {
  //Define variables to hold imu data
  float ax, ay, az, gx, gy, gz;

  //Print data for accelerometer and gyroscope if it's available
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    Serial.print(ax);
    Serial.print('\t');
    Serial.print(ay);
    Serial.print('\t');
    Serial.print(az);
    Serial.print('\t');
    Serial.print(gx);
    Serial.print('\t');
    Serial.print(gy);
    Serial.print('\t');
    Serial.println(gz);
  }
}
