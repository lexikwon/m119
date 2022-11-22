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

const uuid_service = "1102"
//Add in second bluetooth device
const uuid_value = "2106"

let sensorValue2 = NaN

noble.on('stateChange', async (state) => {
  if (state === 'poweredOn') {
    //console.log("start scanning")
    //read out titles
    console.log("Gz");
    await noble.startScanningAsync([uuid_service], false);
  }
});

noble.on('discover', async (peripheral) => {
    console.log("connected pong2")
  await noble.stopScanningAsync();
  await peripheral.connectAsync();
  const {characteristics} = await
peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], [uuid_value]); //Add in reading for the y and z characteristics
  readData(characteristics[0])
});

//
// read data periodically
//
let readData = async (characteristic) => {
  //Add in values for y and z characteristics
  const value = (await characteristic.readAsync());

  sensorValue2 = value.readFloatLE(0);
  //send data to parent
  if(process.send){
      process.send(sensorValue2)
  }

  console.log(value.readFloatLE(0));

  // read data again in t milliseconds
  setTimeout(() => {
    readData(characteristic)
   }, 10);
}

//
// hosting a web-based front-end and respond requests with sensor data
// based on example code on https://expressjs.com/
//
/*
const express = require('express')
const app = express()
const port = 3010

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('pong')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
        //'Access-Control-Allow-Origin': '*'
    });
    res.end(JSON.stringify({
        sensorValue: sensorValue
    }))
})

app.listen(port, () => {
    //console.log(`Example app listening on port ${port}`)
})
*/