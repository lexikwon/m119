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
//Add in second bluetooth device
const uuid_value = "2106"

//set path
var path = require('path')

let sensorValue = NaN
let sensorValue2 = NaN

const fork = require('child_process').fork;
const program = path.resolve('pong2.js');
const parameters = [];
const options = {
  stdio: [ 'pipe', 'pipe', 'pipe', 'ipc' ]
};
const child = fork(program, parameters, options);

child.on('message',message=>{
    sensorValue2 = message;  
    console.log("SENSOR2 VALUE: ",sensorValue2)  
})

noble.on('stateChange', async (state) => {
  if (state === 'poweredOn') {
    //console.log("start scanning")
    //read out titles
    console.log("Gz");
    await noble.startScanningAsync([uuid_service], false);
  }
});

noble.on('discover', async (peripheral) => {
    console.log("CONNECTED PONG1")
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

  sensorValue = value.readFloatLE(0);

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
const express = require('express')
const app = express()
const port = 3000

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
        sensorValue: sensorValue,
        sensorValue2: sensorValue2
    }))
//     res.setHeader("Access-Control-Allow-Origin", "*");
//     res.header(
//     "Access-Control-Allow-Headers",
//     "Origin, X-Requested-With, Content-Type, Accept"
//   );
})

app.listen(port, () => {
    //console.log(`Example app listening on port ${port}`)
})
