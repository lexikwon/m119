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
const bodyParser = require('body-parser')

const uuid_service = "1101"
const uuid_value = "2101"
//Add uuids for y and z characteristics and gyroscope
const uuid_value2 = "2102"
const uuid_value3 = "2103"
const uuid_gvalue = "2104"
const uuid_gvalue2 = "2105"
const uuid_gvalue3 = "2106"
const uuid_slouch = "2107"
const uuid_flexL = "2108"
const uuid_flexR = "2109"

const slouch = Buffer.allocUnsafe(4);
var slouchVal = 0;

let sensorValue = NaN
let sensorValue2 = NaN
let sensorValue3 = NaN
let sensorValue4 = NaN
let sensorValue5 = NaN
let sensorValue6 = NaN
let flexL = NaN
let flexR = NaN

noble.on('stateChange', async (state) => {
  if (state === 'poweredOn') {
    //console.log("start scanning")
    //read out titles
    console.log("Ax,Ay,Az,Gx,Gy,Gz,fL,fR");
    await noble.startScanningAsync([uuid_service], false);
  }
});

noble.on('discover', async (peripheral) => {
  await noble.stopScanningAsync();
  await peripheral.connectAsync();
  const {characteristics} = await 
peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], 
[uuid_value, uuid_value2, uuid_value3, uuid_gvalue, uuid_gvalue2, uuid_gvalue3, uuid_slouch,uuid_flexL,uuid_flexR]); //Add in reading for the y and z characteristics
  readData(characteristics[0],characteristics[1],characteristics[2],characteristics[3],characteristics[4],characteristics[5],characteristics[6],characteristics[7],characteristics[8])
});

//
// read data periodically
//
let readData = async (characteristic, characteristic2, characteristic3, gcharacteristic, gcharacteristic2, gcharacteristic3, slouchchar, flexLchar, flexRchar) => {
  //Add in values for y and z characteristics
  const value = (await characteristic.readAsync());
  const value2 = (await characteristic2.readAsync());
  const value3 = (await characteristic3.readAsync());
  const gvalue = (await gcharacteristic.readAsync());
  const gvalue2 = (await gcharacteristic2.readAsync());
  const gvalue3 = (await gcharacteristic3.readAsync());
  const flexLval = (await flexLchar.readAsync());
  const flexRval = (await flexRchar.readAsync());

  sensorValue = value.readFloatLE(0);
  sensorValue2 = value2.readFloatLE(0);
  sensorValue3 = value3.readFloatLE(0);
  sensorValue4 = gvalue.readFloatLE(0);
  sensorValue5 = gvalue2.readFloatLE(0);
  sensorValue6 = gvalue3.readFloatLE(0);
  flexL = flexLval.readFloatLE(0);
  flexR = flexRval.readFloatLE(0);

  if(slouchVal == 0){
    slouch.writeFloatLE(0x0,0)
  }
  else if(slouchVal == 1){
    slouch.writeFloatLE(0x1,0)
  }
  slouchchar.write(slouch);

  console.log(value.readFloatLE(0)+","+value2.readFloatLE(0)+","+value3.readFloatLE(0)+","+gvalue.readFloatLE(0)+","+gvalue2.readFloatLE(0)+","+gvalue3.readFloatLE(0)+","+flexLval.readFloatLE(0)+","+flexRval.readFloatLE(0));

  // read data again in t milliseconds
  setTimeout(() => {
    readData(characteristic, characteristic2, characteristic3, gcharacteristic, gcharacteristic2, gcharacteristic3, slouchchar, flexLchar, flexRchar)
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
    res.render('index')
})


app.get('/about', (req, res) => {
  res.render('about');
  slouchVal = req.body.slouch
});

app.get('/sensor', (req, res) => {
  res.render('sensor');
  slouchVal = req.body.slouch
});

app.get('/theteam', (req, res) => {
  res.render('theteam');
  slouchVal = req.body.slouch
});


app.use(bodyParser.json());

app.post('*', (req, res) => {

    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        sensorValue: sensorValue,
        sensorValue2: sensorValue2,
        sensorValue3: sensorValue3,
        sensorValue4: sensorValue4,
        sensorValue5: sensorValue5,
        sensorValue6: sensorValue6,
        flexL: flexL,
        flexR: flexR
    }))
    slouchVal = req.body.slouch
})

app.use('/public', express.static('public'));

app.listen(port, () => {
    //console.log(`Example app listening on port ${port}`)
})
