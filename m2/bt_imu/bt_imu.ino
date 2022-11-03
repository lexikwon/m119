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

#define BLE_DEVICE_NAME "Nano 33 IoT"
#define BLE_LOCAL_NAME "Nano 33 IoT"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);

BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);

float x, y, z;

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
  accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  accelerometerService.addCharacteristic(accelerometerCharacteristicZ);

  // add service
  BLE.addService(accelerometerService);

  //Set the initial value for each characteristic
  accelerometerCharacteristicX.writeValue(0.0);
  accelerometerCharacteristicY.writeValue(0.0);
  accelerometerCharacteristicZ.writeValue(0.0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE Accelerometer Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  // obtain and write accelerometer data
  // while the central is still connected to peripheral:
  while(central.connected()){
    //Obtain accelerometer data
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
    }

    //Write accelerometer data
    accelerometerCharacteristicX.writeValue(x);
    accelerometerCharacteristicY.writeValue(y);
    accelerometerCharacteristicZ.writeValue(z);
  }

}
