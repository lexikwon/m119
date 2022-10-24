// based on the example on https://www.npmjs.com/package/@abandonware/noble

/*
 IMU data stream via bluetooth central code
 Code for the device to recieve the IMU data stream using bluetooth
 From m2b_central/test.js by Anthony Chen
 by Alexandra Kwon (UID: 205290932) for ECE M119
*/

//What: m2b_central/test.js
//Where: https://github.com/ucla-hci/m119/blob/main/m2b_central/test.js
//Why: Offers a template for required central project code

const noble = require('@abandonware/noble');

const uuid_service = "1101"
const uuid_value = "2101"

noble.on('stateChange', async (state) => {
  if (state === 'poweredOn') {
    console.log("start scanning")
    await noble.startScanningAsync([uuid_service], false);
  }
});

noble.on('discover', async (peripheral) => {
  await noble.stopScanningAsync();
  await peripheral.connectAsync();
  const {characteristics} = await 
peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], 
[uuid_value]);
  readData(characteristics[0])
});

//
// read data periodically
//
let readData = async (characteristic) => {
  const value = (await characteristic.readAsync());
  console.log(value.readFloatLE(0));

  // read data again in t milliseconds
  setTimeout(() => {
    readData(characteristic)
  }, 10);
}
