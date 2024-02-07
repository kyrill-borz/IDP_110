#include "..\PinDefinitions\pindefinitions.h"
#include <Arduino.h>


void displayBlockType(bool isFoam){ //turn green LED on for foam, red LED on for solid
    if(isFoam){
        int displayLED = greenLedPin;
    }
    else{
        int displayLED = redLedPin;
    }
    digitalWrite(displayLED,HIGH);
}

void resetLED(){ //turn off LEDs after being dropped off
    digitalWrite(greenLedPin,LOW);
    digitalWrite(redLedPin,LOW);
}

void flashLED(){ //flash LED while moving
    digitalWrite(blueLedPin,HIGH);
    delay(500);
    digitalWrite(blueLedPin,LOW);
}

