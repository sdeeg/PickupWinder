#include <SoftwareSerial.h>

//Setup output to Motor Controller
#define rxPin_mc 3  // unused
#define txPin_mc 4  // pin 4 connects to smcSerial RX
SoftwareSerial smc = SoftwareSerial(rxPin_mc, txPin_mc);

//Setup output to Seven Segment Display
#define rxPin_s7s 3 // unused
#define txPin_s7s 2  // pin 4 connects to smcSerial RX
SoftwareSerial s7s = SoftwareSerial(rxPin_s7s, txPin_s7s);

#define potPin    0
#define sensorPin 1

#define triggerUp 200
#define triggerDown 50
#define black 0
#define white 1

void setup() {
  smc.begin(19200);  
  delay(5);               // Delay 5ms for Motor Controller startup to finish (min 1ms) 
  smc.write(0xAA);  // send baud-indicator byte
  smc.write(0x83);  // clear the safe-start violation and let the motor run

  s7s.begin(9600);
  s7s.write(0x76);  // Clear display command
}

char tempString[10];
int potVal = 0;
int speed = 0;
int sensorVal = 0;
int revolutions = 0;
int detector = white;

void loop() {
  // read the pot (0-1023)
  potVal = analogRead(potPin);

  //Buffer the values coming from the pot a little bit.
  if(potVal <=25) { speed = 0; }
  else if(potVal >=1020) { speed = 3200; }
  else { speed = potVal*3.125; }

  // set the motor speed
  //smc.write(0x85);  // motor forward command (this is not the command we're looking for)
  smc.write(0x86);  // motor reverse command
  smc.write(speed & 0x1F);
  smc.write(speed >> 5);

  //read the sensor
  sensorVal = analogRead(sensorPin);
  
  //set the count
  if(sensorVal > triggerUp && detector == white) {
    revolutions++;
    detector = black;
  }
  else if(sensorVal < triggerDown && detector == black) {
    detector = white;
  }

  //display the count
  sprintf(tempString, "%4d", revolutions);
  s7s.print(tempString);
}
