
void displayBlockType(bool isFoam, int greenLedPin, int redLedPin){ //turn green LED on for foam, red LED on for solid
    int displayLED;
    if(isFoam){
        displayLED = greenLedPin;
    }
    else{
        displayLED = redLedPin;
    }
    digitalWrite(displayLED,1);
}

void resetLED(int greenLedPin,int redLedPin){ //turn off LEDs after being dropped off
    digitalWrite(greenLedPin,0);
    digitalWrite(redLedPin,0);
}

void flashLED(int blueLedPin){ //flash LED while moving
    digitalWrite(blueLedPin, (millis() / 1000) % 2);;
}

