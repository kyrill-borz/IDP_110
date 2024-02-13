#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <stdlib.h>
#include "Pathfinding/pathfinding.hpp"
#include "PinDefinitions/pindefinitions.hpp"
#include "Grabbing/grabbing.hpp"
#include "BlockSense/blockSense.hpp"
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
bool BlockStatus = 0;
int pathlist[] = {3,9, 7, 11};
int stage = 0;
int isFoam;
// Variable to check if button has been pressed
bool buttonPressed = false;


// Define Motors
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

Servo armServo; // create servo object to control a servo
Servo gripServo;

void (* resetFunc) (void) = 0;

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
 armServo.writeMicroseconds(500);
 gripServo.writeMicroseconds(500);
 attachInterrupt(digitalPinToInterrupt(rightlinesensorPin),stopRightTurn,RISING); //interrupts triggered by front line sensors to stop turning
 attachInterrupt(digitalPinToInterrupt(leftlinesensorPin),stopLeftTurn,RISING);
 attachInterrupt(digitalPinToInterrupt(pushButton),SwitchButtonState,RISING);
}

// ############################################ FUNCTIONS ###########################

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin); //Sees whether junction has been hit
  return Junct;
}

void SwitchButtonState(){
  buttonPressed = not buttonPressed;
}


void turnLeft(){ //adjust turning functions to match motor orientations
  delay(300);
  turningLeft = 1;
  while(turningLeft == 1){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
  }
}

void turnRight(){ 
  delay(300);
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
  JunctionStatus = 0;
  do {
    JunctionStatus = JunctionSense();
    flashLED(blueLedPin);
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
  
 RightMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
    RightMotor->setSpeed(valLeft*200);
     // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
    LeftMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*200);
 //delay(1);
  } while (JunctionStatus == 0); // stops when a junction is hit
};

void FindBlock(){
  LeftMotor->setSpeed(0);
  RightMotor->setSpeed(0);
  setServoAngle(gripServo, 50);
  delay(100);
  do {
  BlockStatus = senseBlockIR(sensor);
  flashLED(blueLedPin);
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
  RightMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
    RightMotor->setSpeed(valLeft*130);
     // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
    LeftMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*130);
  delay(50);
  } while (BlockStatus == 0); // stops when a junction is hit
};

void PickUpBlock(){
  LeftMotor->setSpeed(0);
  RightMotor->setSpeed(0);
  setServoAngle(gripServo, 50);
  delay(1000);
  setServoAngle(armServo, 0);
  delay(1000);
  setServoAngle(gripServo, 0, 4);
  delay(1000);
  setServoAngle(armServo, 30);
  IdentifyBlock();
  delay(5000);
}

void PutDownBlock(){
  LeftMotor->setSpeed(0);
  RightMotor->setSpeed(0);  
  setServoAngle(gripServo, 50);
  delay(1000);
  setServoAngle(armServo, 30);
  resetLED(greenLedPin, redLedPin);
}

void SpinAround(){
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
  LeftMotor->setSpeed(100);
  RightMotor->setSpeed(100);
  delay(200);
  turnRight();
}
void IdentifyBlock(){
  isFoam = blockTypeIR(sensor);
  displayBlockType(isFoam, greenLedPin, redLedPin);
}

void DropOffBlock(int location){
  int deliveryLocation;
  if (isFoam == 1){
    deliveryLocation = 12;
  } else {
    deliveryLocation = 13;
  };
  SpinAround();
  String path = ConvertToLocalPath(GetPathToTarget(location, deliveryLocation));
  path.remove(0,1);
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
    PutDownBlock();
    LeftMotor->run(FORWARD);
    RightMotor->run(FORWARD);
    LeftMotor->setSpeed(100);
    RightMotor->setSpeed(100);
    delay(3000);
    turnRight();
  // Deals with the block and returns to the start before generating the next path
  }
void EnterDepo(){
}

void ScanForWarehouseBlock(){
  do {
    BlockStatus += senseBlockIR(sensor);
    flashLED(blueLedPin);

    LeftMotor->run(BACKWARD);
    LeftMotor->setSpeed(((1 - (millis() / 2000) % 2)) * 100);
    RightMotor->run(FORWARD);
    RightMotor->setSpeed(((millis() / 2000) % 2) * 100);
 //delay(1);
  } while (BlockStatus == 0); // stops when a junction is hit
};

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
    int deliveryLocation;
    if (isFoam == 1){
      deliveryLocation = 12;
    } else {
      deliveryLocation = 13;
    }
    String path = ConvertToLocalPath(GetPathToTarget(deliveryLocation, 0));
    
    path.remove(0,1);
    //path += "C";
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
    setServoAngle(armServo, 0);
    EnterDepo();
}

void enterWarehouse(int stage){
  int time_taken = 0;
  do {
  flashLED(blueLedPin);
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
  RightMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
    RightMotor->setSpeed(valLeft*130);
     // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
    LeftMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*130);
  delay(50);
  time_taken += 50;
  } while (time_taken <= 5000); // stops when a junction is hit
  if (stage == 2) {
      int time_taken_turn = 0;
      while(time_taken_turn<= 1000){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
    time_taken_turn += 50;
    delay(50);
  }
  } else {
      int time_taken_turn = 0;
      while(time_taken_turn<= 1000){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(FORWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(BACKWARD);
    LeftMotor->setSpeed(200);
    time_taken_turn += 50;
    delay(50);
  }
  }
}
// ############################# MAIN LOOP ########################
// Arm angles are 0 and 30 for down and up 
// Grab angles are 0 50 for completely closed and open

void loop(){
  if (buttonPressed) { 
     if(stage<=1){
      Serial.print("Starting");
    
    String path = ConvertToLocalPath(GetPathToTarget(0, pathlist[stage]));
    int directionsLength = path.length(); //path.size();
    if (stage == 0){LeaveBox();};
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
    }
  // Deals with the block and returns to the start before generating the next path
    FindBlock();
    PickUpBlock();
    buttonPressed = true;
    DropOffBlock(pathlist[stage]);
    ReturnToDepo();
    stage += 1;
     } else {
      String path = ConvertToLocalPath(GetPathToTarget(0, pathlist[stage]));
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength; i++){ //Loops through each direction until the block is reached
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
    turnRight();
    enterWarehouse(stage);
    ScanForWarehouseBlock();
    PickUpBlock();
      int time_taken_turn = 0;
      while(time_taken_turn<= 1000){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
    time_taken_turn += 50;
    delay(50);
    }
    LeaveBox();
    DropOffBlock(7);
    ReturnToDepo();
     }}}
};
