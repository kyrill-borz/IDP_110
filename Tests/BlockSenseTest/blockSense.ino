#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;

#define max_range (520)
#define adc (1023.0)
int ultrasoundPin = A0;

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

bool senseBlockIR(){ //sensing block distance using IR
    float blockDistIR = sensor.getDistance();
    if(blockDist > 80){
        return 0;
    }
    else{
        return 1;
    }
}

bool senseBlockUS(){ //sensing block using ultrasound
    float sense = analogRead(ultrasoundPin);
    float dist  = sense * max_range / adc;
    if(dist < 2){
        return 1;
    }
    else{
        return 0;
    }

}

bool blockTypeIR(){//determing block type using IR
    float blockTypeDist = sensor.getDistance();
    if(blockTypeDist < 60){ //calibrate threshold for block identification
        return 1 //return 1 for a solid block
    }
    else{
        return 0 //return 0 for a foam block
    }
}
