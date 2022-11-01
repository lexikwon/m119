/*
 IMU data stream via bluetooth central code
 Code for the device to recieve the IMU data stream using bluetooth and visualizes the data
 Adapted from m3/central.js by Anthony Chen
 by Alexandra Kwon (UID: 205290932) for ECE M119
*/

//What: m3/central.js
//Where: https://github.com/ucla-hci/m119/blob/main/m3/central.js
//Why: Offers a template for required project code

// based on the example on https://www.npmjs.com/package/@abandonware/noble

const noble = require('@abandonware/noble');

const uuid_service = "1101"
const uuid_value = "2101"
//Add uuids for y and z characteristics
const uuid_value2 = "2102"
const uuid_value3 = "2103"

let sensorValue = NaN
let sensorValue2 = NaN
let sensorValue3 = NaN

noble.on('stateChange', async (state) => {
    if (state === 'poweredOn') {
        console.log("start scanning")
        await noble.startScanningAsync([uuid_service], false);
    }
});

noble.on('discover', async (peripheral) => {
    await noble.stopScanningAsync();
    await peripheral.connectAsync();
    const {
        characteristics
    } = await peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], [uuid_value, uuid_value2, uuid_value3]);
    readData(characteristics[0],characteristics[1],characteristics[2])
});

//
// read data periodically
//
let readData = async (characteristic, characteristic2, characteristic3) => {
    const value = (await characteristic.readAsync());
    const value2 = (await characteristic2.readAsync());
    const value3 = (await characteristic3.readAsync());
    sensorValue = value.readFloatLE(0);
    sensorValue2 = value2.readFloatLE(0);
    sensorValue3 = value3.readFloatLE(0);
    console.log(sensorValue, sensorValue2, sensorValue3);

    // read data again in t milliseconds
    setTimeout(() => {
        readData(characteristic, characteristic2, characteristic3)
    }, 10);
}

//
// hosting a web-based front-end and respond requests with sensor data
// based on example code on https://expressjs.com/
//
const express = require('express')
const app = express()
const port = 3090

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('index')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        sensorValue: sensorValue,
        sensorValue2: sensorValue2,
        sensorValue3: sensorValue3
    }))
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})