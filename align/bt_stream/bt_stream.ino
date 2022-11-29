/*
  IMU data and flex sensor stream via bluetooth with vibration motor

  Streams IMU data and flex sensor data to another device using bluetooth, also recieves data and vibrates motor

  Adapted from m2b_peripheral.ino by Anthony Chen and Flex_Sensor_Example.ino by Jim Lindblom

  by Alexandra Kwon (UID: 205290932) for ECE M119
*/

//What: m2b_peripheral.ino
//Where: https://github.com/ucla-hci/m119/blob/main/m2b_peripheral/m2b_peripheral.ino
//Why: Offers a template for required project code

//What: Flex_Sensor_Example.ino
//Where: https://learn.sparkfun.com/tutorials/flex-sensor-hookup-guide/all
//Why: Offers a template for reading flex sensor values



#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define BLE_UUID_ACCELEROMETER_SERVICE "1101"
#define BLE_UUID_ACCELEROMETER_X "2101"
//#define BLE_UUID_ACCELEROMETER_Y "2102"
//#define BLE_UUID_ACCELEROMETER_Z "2103"
//#define BLE_UUID_GYROSCOPE_X "2104"
#define BLE_UUID_GYROSCOPE_Y "2105"
//#define BLE_UUID_GYROSCOPE_Z "2106"
#define BLE_UUID_SLOUCH "2107"
//#define BLE_UUID_FLEXL "2108"
//#define BLE_UUID_FLEXR "2109"

#define BLE_DEVICE_NAME "Nano 33 IoT"
#define BLE_LOCAL_NAME "Nano 33 IoT"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);

int value = 0;

BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
//BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
//BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);
//BLEFloatCharacteristic gyroscopeCharacteristicX(BLE_UUID_GYROSCOPE_X, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicY(BLE_UUID_GYROSCOPE_Y, BLERead | BLENotify);
//BLEFloatCharacteristic gyroscopeCharacteristicZ(BLE_UUID_GYROSCOPE_Z, BLERead | BLENotify);
BLEIntCharacteristic slouchCharacteristic(BLE_UUID_SLOUCH, BLERead | BLEWrite);
//BLEFloatCharacteristic flexLCharacteristic(BLE_UUID_FLEXL, BLERead | BLENotify);
//BLEFloatCharacteristic flexRCharacteristic(BLE_UUID_FLEXR, BLERead | BLENotify);

float ax, ay, az, gx, gy, gz;
const int motorPin = 2; //motor transistor is connected to pin 2
//const int FLEX_PIN_L = A1; // Pin connected to voltage divider output
//const int FLEX_PIN_R = A0; // Pin connected to voltage divider output

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
//const float VCC = 3.33; // Measured voltage of Ardunio 5V line
//const float R_DIV = 72600.0; // Measured resistance of 100k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
//const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
//const float BEND_RESISTANCE = 100000.0; // resistance at 90 deg

void setup() {
  pinMode(motorPin, OUTPUT);
  //pinMode(FLEX_PIN_L, INPUT);
  //pinMode(FLEX_PIN_R, INPUT);

  //Serial.begin(9600);
  //while (!Serial)
    ;

  // initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");

  // initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

  // set advertised local name and service UUID
  BLE.setLocalName(BLE_LOCAL_NAME);
  BLE.setAdvertisedService(accelerometerService);

  // add characteristics to the service
  accelerometerService.addCharacteristic(accelerometerCharacteristicX);
  //accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  //accelerometerService.addCharacteristic(accelerometerCharacteristicZ);
  //accelerometerService.addCharacteristic(gyroscopeCharacteristicX);
  accelerometerService.addCharacteristic(gyroscopeCharacteristicY);
  //accelerometerService.addCharacteristic(gyroscopeCharacteristicZ);
  accelerometerService.addCharacteristic(slouchCharacteristic);
  //accelerometerService.addCharacteristic(flexLCharacteristic);
  //accelerometerService.addCharacteristic(flexRCharacteristic);

  // add service
  BLE.addService(accelerometerService);

  //Set the initial value for each characteristic
  accelerometerCharacteristicX.writeValue(0.0);
  //accelerometerCharacteristicY.writeValue(0.0);
  //accelerometerCharacteristicZ.writeValue(0.0);
  //gyroscopeCharacteristicX.writeValue(0.0);
  gyroscopeCharacteristicY.writeValue(0.0);
  //gyroscopeCharacteristicZ.writeValue(0.0);
  //flexLCharacteristic.writeValue(0.0);
  //flexRCharacteristic.writeValue(0.0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE IMU Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  // obtain and write accelerometer data
  // while the central is still connected to peripheral:
  while(central.connected()){
    //Obtain accelerometer data
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      IMU.readAcceleration(ax, ay, az);
      IMU.readGyroscope(gx, gy, gz);
    }

    //Write accelerometer data
    accelerometerCharacteristicX.writeValue(ax);
    //accelerometerCharacteristicY.writeValue(ay);
    //accelerometerCharacteristicZ.writeValue(az);
    //gyroscopeCharacteristicX.writeValue(gx);
    gyroscopeCharacteristicY.writeValue(gy);
    //gyroscopeCharacteristicZ.writeValue(gz);

    /*
    // Read the ADC, and calculate voltage and resistance from it
    int flexLADC = analogRead(FLEX_PIN_L);
    float flexLV = flexLADC * VCC / 1023.0;
    float flexLR = R_DIV * (VCC / flexLV - 1.0);

    // Use the calculated resistance to estimate the sensor's
    // bend angle:
    float angleL = map(flexLR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                    0, 90.0);
    flexLCharacteristic.writeValue(angleL);

    // Read the ADC, and calculate voltage and resistance from it
    int flexRADC = analogRead(FLEX_PIN_R);
    float flexRV = flexRADC * VCC / 1023.0;
    float flexRR = R_DIV * (VCC / flexRV - 1.0);

    // Use the calculated resistance to estimate the sensor's
    // bend angle:
    float angleR = map(flexRR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                    0, 90.0);
    flexRCharacteristic.writeValue(angleR);
    */

    //Buzz vibration motor if slouch is detected
    if(slouchCharacteristic.written()){
      value = slouchCharacteristic.value();
      Serial.println(value);
      if(value!=0){
        digitalWrite(motorPin, HIGH); //vibrate
      }
      else{
        digitalWrite(motorPin,LOW); //stop vibrating
      }
    }

  }

  //Stop vibrating when done with the code
  digitalWrite(motorPin,LOW);

}
