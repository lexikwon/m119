/*
  Hello IMU

  Flashes the on board LED in morse code to spell "HELLO IMU"
  In this code, "one unit" = 0.5 sec

  by Alexandra Kwon (UID: 205290932) for ECE M119
*/
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void dash(){
  //Displays a "dash" in morse code on the onboard LED
  //Turn LED on
  digitalWrite(LED_BUILTIN, HIGH); 
  //Wait for 1.5 seconds (3 units, dash)  
  delay(1500);                       
  //Turn LED off
  digitalWrite(LED_BUILTIN, LOW);
  //Wait for 0.5 seconds (one unit, minimum length between dashes/dots)
  delay(500);
}

void dot(){
  //Displays a "dot" in morse code on the onboard LED
  //Turn LED on
  digitalWrite(LED_BUILTIN, HIGH); 
  //Wait for 0.5 seconds (1 unit, dot)  
  delay(500);                       
  //Turn LED off
  digitalWrite(LED_BUILTIN, LOW);
  //Wait for 0.5 seconds (one unit, minimum length between dashes/dots)
  delay(500);
}

void hello(){
  //Write "HELLO"
  //H
  dot();
  dot();
  dot();
  dot();
  //Wait for 2 more units (space between letters)
  delay(1000);
  //E
  dot();
  //Wait for 2 more units (space between letters)
  delay(1000);
  //L
  dot();
  dash();
  dot();
  dot();
  //Wait for 2 more units (space between letters)
  delay(1000);
  //L
  dot();
  dash();
  dot();
  dot();
  //Wait for 2 more units (space between letters)
  delay(1000);
  //O
  dash();
  dash();
  dash();
  //Wait for 2 more units (space between letters)
  delay(1000);
}

void imu(){
  //I
  dot();
  dot();
  //Wait for 2 more units (space between letters)
  delay(1000);
  //M
  dash();
  dash();
  //Wait for 2 more units (space between letters)
  delay(1000);
  //U
  dot();
  dot();
  dash();
  //Wait for 2 more units (space between letters)
  delay(1000);
}

void loop() {
  //Write hello
  hello();
  //Wait four more units (space between words)
  delay(2000);
  //Write IMU
  imu();
  //Wait four more units (space between words)
  delay(2000);
}
