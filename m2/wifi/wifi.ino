/*
  Code to connect the Nano 33 IoT to a Wi-Fi network

  Adapted from Connecting the Nano 33 IoT to a Wi-Fi Network by Fabricio Troya

  by Alexandra Kwon (UID: 205290932) for ECE M119
*/

//What: Controlling built-in LED through bluetooth
//Where: https://docs.arduino.cc/tutorials/nano-33-iot/bluetooth
//Why: Offers a comprehensive tutorial on how to connect the Nano 33 via bluetooth to be further used in future projects

//Include the WiFi NINA library
#include <WiFiNINA.h>
//Include "secret" file with private information
#include "secret.h"

//Initialize the secret variables
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char user[] = SECRET_USER;

//Initialize more variables to store WiFi information
//the WiFi radio's status
int status = WL_IDLE_STATUS;
//ledState used to set the LED
int ledState = LOW;
//will store last time Wi-Fi information was pudated
unsigned long previousMillisInfo = 0;
//will store the last time LED was updated
unsigned long previousMillisLED = 0;
//interval at which to update the board information
const int intervalInfo = 5000;

void setup() {
  //start the serial communication at 9600 bauds
  Serial.begin(9600);
  while(!Serial);

  //Set the LED as output
  pinMode(LED_BUILTIN, OUTPUT);

  //Create a while loop to check if we are connected to Wi-Fi and begin connecting to it
  while(status != WL_CONNECTED){
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    //Connect to WPA/WPA2 netowrk;
    status = WiFi.beginEnterprise(ssid,user,pass);

    //wait 10 seconds for connection
    delay(10000);
    Serial.println(status);
  }

  //Print out message to signify connected to the WiFi network
  Serial.println("You're connected to the network");
  Serial.println("--------------------------------");
}

void loop() {
  //Initialize a new variable that will store the time since sketch started running
  unsigned long currentMillisInfo = millis();

  //Check if the time after the last update is bigger than the interval
  if(currentMillisInfo - previousMillisInfo >= intervalInfo){
    Serial.println("Board Information:");
    //Print board's IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    //Print network's SSID:
    Serial.println();
    Serial.println("Network Information:");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    //Print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);
    Serial.println("---------------------------------------");
  }

  unsigned long currentMillisLED = millis();

  //Measure the signal strength and convert it into a time interval
  int intervalLED = WiFi.RSSI() * -10;

  //Check if the time after the last blink is bigger the interval 
  if (currentMillisLED - previousMillisLED >= intervalLED) {
    previousMillisLED = currentMillisLED;

    //If the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    //Set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }

  //Delay so you can properly see the network information
  delay(1000);
}
