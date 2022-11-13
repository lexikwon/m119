/*
 IMU data stream via bluetooth central code
 Code for the device to recieve the IMU data stream using bluetooth and uses data to play pong
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

let sensorValue = NaN

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
    } = await peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], [uuid_value]);
    readData(characteristics[0])
});

//
// read data periodically
//
let readData = async (characteristic) => {
    const value = (await characteristic.readAsync());
    sensorValue = value.readFloatLE(0);
    console.log(sensorValue);

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
const port = 3090

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('pong')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        sensorValue: sensorValue
    }))
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})