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
//Add uuids for y and z characteristics and gyroscope
const uuid_value2 = "2102"
const uuid_value3 = "2103"
const uuid_gvalue = "2104"
const uuid_gvalue2 = "2105"
const uuid_gvalue3 = "2106"

noble.on('stateChange', async (state) => {
  if (state === 'poweredOn') {
    //console.log("start scanning")
    //read out titles
    console.log("Ax, Ay, Az, Gx, Gy, Gz");
    await noble.startScanningAsync([uuid_service], false);
  }
});

noble.on('discover', async (peripheral) => {
  await noble.stopScanningAsync();
  await peripheral.connectAsync();
  const {characteristics} = await 
peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], 
[uuid_value, uuid_value2, uuid_value3, uuid_gvalue, uuid_gvalue2, uuid_gvalue3]); //Add in reading for the y and z characteristics
  readData(characteristics[0],characteristics[1],characteristics[2],characteristics[3],characteristics[4],characteristics[5])
});

//
// read data periodically
//
let readData = async (characteristic, characteristic2, characteristic3, gcharacteristic, gcharacteristic2, gcharacteristic3) => {
  //Add in values for y and z characteristics
  const value = (await characteristic.readAsync());
  const value2 = (await characteristic2.readAsync());
  const value3 = (await characteristic3.readAsync());
  const gvalue = (await gcharacteristic.readAsync());
  const gvalue2 = (await gcharacteristic2.readAsync());
  const gvalue3 = (await gcharacteristic3.readAsync());

  console.log(value.readFloatLE(0)+","+value2.readFloatLE(0)+","+value3.readFloatLE(0)+","+gvalue.readFloatLE(0)+","+gvalue2.readFloatLE(0)+","+gvalue3.readFloatLE(0));

  // read data again in t milliseconds
  setTimeout(() => {
    readData(characteristic, characteristic2, characteristic3, gcharacteristic, gcharacteristic2, gcharacteristic3)
   }, 10);
}
