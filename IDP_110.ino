#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <stdlib.h>
#include "pathfinding\pathfinding.h"
#include "PinDefinitions\pindefinitions.h"
#include "Movement\movement.h"

using namespace std;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// ######################################### SETUP #################################


// Variable to check if button has been pressed
bool buttonPressed = false;

// Define Motors
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

Servo armServo; // create servo object to control a servo

void setup() {
 
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
 Serial.begin(9600); // Init the serial port

 // Declare Input pins

 pinMode(leftlinesensorPin, INPUT); 
 pinMode(rightlinesensorPin, INPUT); 
 pinMode(leftjunctionsensorPin, INPUT);
 pinMode(rightjunctionsensorPin, INPUT);
 pinMode(crashswitchPin, INPUT);
 pinMode(pushButton, INPUT);

 armServo.attach(9); // attaches the servo on pin 9 to the servo object
}

// ############################################ FUNCTIONS ###########################

void SwitchButtonState(){
  buttonPressed = not buttonPressed;
}

void PickUpBlock(){
  int positionofservo = 0; //resets servo angle
  do {
    positionofservo += 1;
    armServo.write(positionofservo); // Keeps tightening servo arm until the block is grabbed
    delay(15);
  } while (digitalRead(crashswitchPin) == LOW);
  
}

void FindBlock(){

}

void IdentifyBlock(){
  
}

void DropOffBlock(){

}

void ReturnToDepo(){
  
}

// ############################# MAIN LOOP ########################

void loop(){
//  //String path[] = generatePath(); //gets a list of directions
  attachInterrupt(digitalPinToInterrupt(pushButton),SwitchButtonState,RISING);
  if (buttonPressed) { 
    String path = ConvertToLocalPath(GetPathToTarget(0, 9));
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength; i++){ //Loops through each direction until the block is reached
      MoveToNextJunction(); // follows the line to next junction
      attachInterrupt(digitalPinToInterrupt(rightlinesensorPin),stopRightTurn,RISING); //interrupts triggered by front line sensors to stop turning
      attachInterrupt(digitalPinToInterrupt(leftlinesensorPin),stopLeftTurn,RISING);
    if (path[i] == 'L'){ // decides what to do at each junction
      turnLeft();
      delay(100);
    } else if (path[i] == 'R'){
      turnRight();
      delay(100);
    } else {
      delay(300);
    };

    detachInterrupt(digitalPinToInterrupt(leftjunctionsensorPin)); //interrupts triggered by front line sensors to stop turning
    detachInterrupt(digitalPinToInterrupt(rightjunctionsensorPin));
    }
  // Deals with the block and returns to the start before generating the next path
    FindBlock();
    PickUpBlock();
    IdentifyBlock();
    DropOffBlock();
    ReturnToDepo();
  }
};
