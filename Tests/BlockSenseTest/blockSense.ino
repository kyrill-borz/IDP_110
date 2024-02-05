#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;

float initDist;
float distChange;
float currDist;
float distToBlock = 35;
void setup() {
 //initialize serial communication at 9600 bits per second:
 Serial.begin(9600);
 //join i2c bus (address optional for master)
 Wire.begin();
 //Set I2C sub-device address
 sensor.begin(0x50);
 //Set to Back-to-back mode and high precision mode
 sensor.setMode(sensor.eContinuous,sensor.eHigh);
 //Laser rangefinder begins to work
 sensor.start();
}

bool senseBlock(){ //
    float blockDist = sensor.getDistance();
    if(blockDist > 50){
        return 0;
    }
    else{
        return 1;
    }
}
void loop(){
    senseBlock();
}