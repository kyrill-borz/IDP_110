#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <stdlib.h>
#include "Pathfinding/pathfinding.hpp"
#include "PinDefinitions/pindefinitions.hpp"
#include "Grabbing/grabbing.hpp"
#include "LEDs/LEDs.hpp"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"


using namespace std;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

DFRobot_VL53L0X sensor;
// ######################################### SETUP #################################

// Store variables for turning
int turningLeft;
int turningRight;
int JunctionStatus = 0;
int BlockStatus = 0;
int pathlist[] = {3,9};
int stage = 0;
// Variable to check if button has been pressed
bool buttonPressed = false;


// Define Motors
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

Servo armServo; // create servo object to control a servo
Servo gripServo;

void setup() {
 
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");

    while (1);
  }
 Serial.begin(9600); // Init the serial port
  // Declare Input pins
  Wire.begin();
  //Set I2C sub-device address
  sensor.begin(0x50);
  //Set to Back-to-back mode and high precision mode
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  //Laser rangefinder begins to work
  sensor.start();
  
 pinMode(leftlinesensorPin, INPUT); 
 pinMode(rightlinesensorPin, INPUT); 
 pinMode(leftjunctionsensorPin, INPUT);
 pinMode(rightjunctionsensorPin, INPUT);
 pinMode(crashswitchPin, INPUT);
 pinMode(pushButton, INPUT);
 armServo.attach(armPin); // attaches the servo on pin 9 to the servo object
 gripServo.attach(grabberPin);
 attachInterrupt(digitalPinToInterrupt(rightlinesensorPin),stopRightTurn,RISING); //interrupts triggered by front line sensors to stop turning
 attachInterrupt(digitalPinToInterrupt(leftlinesensorPin),stopLeftTurn,RISING);
 attachInterrupt(digitalPinToInterrupt(pushButton),SwitchButtonState,RISING);
}

// ############################################ FUNCTIONS ###########################

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin); //Sees whether junction has been hit
  return Junct;
}

int BlockSense(){
  return 1;
}
void SwitchButtonState(){
  buttonPressed = not buttonPressed;
}

int senseBlock(){ //
    float blockDist = sensor.getDistance();
    if(blockDist > 100){
        return 0;
    }
    else{
        return 1;
    }
}

void turnLeft(){ //adjust turning functions to match motor orientations
  delay(200);
  turningLeft = 1;
  while(turningLeft == 1){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
  }
}

void turnRight(){ 
  delay(200);
  turningRight = 1;
  while(turningRight == 1){ // defines a loop for turning to the right until interrupt is hit
    LeftMotor->run(BACKWARD);
    LeftMotor->setSpeed(200);
    RightMotor->run(FORWARD);
    RightMotor->setSpeed(200);
  }
};

void stopLeftTurn(){  //functions called by Interrupts
  turningLeft = 0;
}

void stopRightTurn(){
  turningRight = 0;
}


void MoveToNextJunction(){
  do {
    JunctionStatus = JunctionSense();
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
 Serial.print(valLeft);
 RightMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
    RightMotor->setSpeed(valLeft*200);
     // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.print(valRight);
    LeftMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*200);
 //delay(1);
  } while (JunctionStatus == 0); // stops when a junction is hit
};

void FindBlock(){
  do {
  BlockStatus = senseBlock();
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
  Serial.print(valLeft);
  RightMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
    RightMotor->setSpeed(valLeft*200);
     // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.print(valRight);
    LeftMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*200);
  } while (BlockStatus == 0); // stops when a junction is hit
};

void PickUpBlock(){
  LeftMotor->setSpeed(0);
  RightMotor->setSpeed(0);
  lowerArm(gripServo, armServo);
  grabBlock(gripServo, armServo);
  liftArm(gripServo, armServo);
  
}

void SpinAround(){
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
  LeftMotor->setSpeed(100);
  RightMotor->setSpeed(100);
  delay(1000);
  turnRight();
}
void IdentifyBlock(){
  
}

void DropOffBlock(int location){
  String path = ConvertToLocalPath(GetPathToTarget(location, 12));
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength; i++){ //Loops through each direction until the block is reached
      MoveToNextJunction(); // follows the line to next junction
    if (path[i] == 'L'){ // decides what to do at each junction
      turnLeft();
      delay(100);
    } else if (path[i] == 'R'){
      turnRight();
      delay(100);
    } else if (path[i] == 'B'){
      SpinAround();
      delay(100);
    } else {
      delay(300);
    };
    LeftMotor->setSpeed(0);
    RightMotor->setSpeed(0);
    }
    lowerArm(gripServo, armServo);
    dropBlock(gripServo, armServo);
    liftArm(gripServo, armServo);
    SpinAround();
  // Deals with the block and returns to the start before generating the next path
  }
void EnterDepo(){
  SpinAround();
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
  LeftMotor->setSpeed(100);
  RightMotor->setSpeed(100);
  delay(2000);
  LeftMotor->setSpeed(0);
  RightMotor->setSpeed(0);

}
void LeaveBox(){
    do {
    JunctionStatus = JunctionSense();
    LeftMotor->run(BACKWARD);
    RightMotor->run(BACKWARD);
    LeftMotor->setSpeed(100);
    RightMotor->setSpeed(100);
    } while (JunctionStatus == 0);
    JunctionStatus = 0;
    delay(100);
}
void ReturnToDepo(){
    String path = ConvertToLocalPath(GetPathToTarget(12, 0));
    path += "C";
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength-1; i++){ //Loops through each direction until the block is reached
      MoveToNextJunction(); // follows the line to next junction
    if (path[i] == 'L'){ // decides what to do at each junction
      turnLeft();
      delay(100);
    } else if (path[i] == 'R'){
      turnRight();
      delay(100);
    }else {
      delay(300);
    };
    LeftMotor->setSpeed(0);
    RightMotor->setSpeed(0);
    }
    EnterDepo();
}

// ############################# MAIN LOOP ########################


void loop(){
  attachInterrupt(digitalPinToInterrupt(rightlinesensorPin),stopRightTurn,RISING); //interrupts triggered by front line sensors to stop turning
  attachInterrupt(digitalPinToInterrupt(leftlinesensorPin),stopLeftTurn,RISING);
  attachInterrupt(digitalPinToInterrupt(pushButton),SwitchButtonState,RISING);
  if (buttonPressed) { 
    Serial.print("Starting");
    LeaveBox();
    String path = ConvertToLocalPath(GetPathToTarget(0, pathlist[stage]));
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength-1; i++){ //Loops through each direction until the block is reached
      MoveToNextJunction(); // follows the line to next junction
    if (path[i] == 'L'){ // decides what to do at each junction
      turnLeft();
      delay(100);
    } else if (path[i] == 'R'){
      turnRight();
      delay(100);
    } else if (path[i] == 'B')
    {
      SpinAround();
      delay(100);
    }else {
      delay(300);
    };
    LeftMotor->setSpeed(0);
    RightMotor->setSpeed(0);
    }
  // Deals with the block and returns to the start before generating the next path
    FindBlock();
    PickUpBlock();
    //IdentifyBlock();
    buttonPressed = false;
    DropOffBlock(pathlist[stage]);
    ReturnToDepo();
    stage += 1;
  }
};