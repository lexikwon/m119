/*
  IMU data stream via bluetooth

  Streams IMU data to another device using bluetooth

  Adapted from m2b_peripheral.ino by Anthony Chen

  by Alexandra Kwon (UID: 205290932) for ECE M119
*/

//What: m2b_peripheral.ino
//Where: https://github.com/ucla-hci/m119/blob/main/m2b_peripheral/m2b_peripheral.ino
//Why: Offers a template for required project code

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define BLE_UUID_ACCELEROMETER_SERVICE "1101"
#define BLE_UUID_ACCELEROMETER_X "2101"
#define BLE_UUID_ACCELEROMETER_Y "2102"
#define BLE_UUID_ACCELEROMETER_Z "2103"
#define BLE_UUID_GYROSCOPE_X "2104"
#define BLE_UUID_GYROSCOPE_Y "2105"
#define BLE_UUID_GYROSCOPE_Z "2106"

#define BLE_DEVICE_NAME "Nano 33 IoT"
#define BLE_LOCAL_NAME "Nano 33 IoT"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);

BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
/*
BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicX(BLE_UUID_GYROSCOPE_X, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicY(BLE_UUID_GYROSCOPE_Y, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicZ(BLE_UUID_GYROSCOPE_Z, BLERead | BLENotify);
*/

float ax, ay, az, gx, gy, gz;


BLEDevice central = BLE.central();

void setup() {

  //pinMode(LED_BUILTIN, OUTPUT);

  //Serial.begin(9600);
  //while (!Serial)
    //;

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
  /*
  accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  accelerometerService.addCharacteristic(accelerometerCharacteristicZ);
  accelerometerService.addCharacteristic(gyroscopeCharacteristicX);
  accelerometerService.addCharacteristic(gyroscopeCharacteristicY);
  accelerometerService.addCharacteristic(gyroscopeCharacteristicZ);
  */

  // add service
  BLE.addService(accelerometerService);

  //Set the initial value for each characteristic
  accelerometerCharacteristicX.writeValue(0.0);
  /*
  accelerometerCharacteristicY.writeValue(0.0);
  accelerometerCharacteristicZ.writeValue(0.0);
  gyroscopeCharacteristicX.writeValue(0.0);
  gyroscopeCharacteristicY.writeValue(0.0);
  gyroscopeCharacteristicZ.writeValue(0.0);
  */

  // start advertising
  BLE.advertise();

  Serial.println("BLE IMU Peripheral");
}

void loop() {

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
    /*
    accelerometerCharacteristicY.writeValue(ay);
    accelerometerCharacteristicZ.writeValue(az);
    gyroscopeCharacteristicX.writeValue(gx);
    gyroscopeCharacteristicY.writeValue(gy);
    gyroscopeCharacteristicZ.writeValue(gz);
    */

  }

}
