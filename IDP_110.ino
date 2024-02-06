#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <stdlib.h>
#include "Pathfinding/pathfinding.hpp"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"


using namespace std;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

DFRobot_VL53L0X sensor;
// ######################################### SETUP #################################

// Calibrate input pins
int leftlinesensorPin = 9;
int rightlinesensorPin = 8; 
int leftjunctionsensorPin = 10;
int rightjunctionsensorPin = 7;
int crashswitchPin = 3; 
int pushButton = 5;

// Store variables for turning
int turningLeft;
int turningRight;
int JunctionStatus = 0;
int BlockStatus = 0;

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
 armServo.attach(9); // attaches the servo on pin 9 to the servo object
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

bool senseBlock(){ //
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
 //delay(1);
  } while (BlockStatus == 0); // stops when a junction is hit
};
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

void PickUpBlock(){
  LeftMotor->setSpeed(0);
  RightMotor->setSpeed(0);

  int positionofservo = 0; //resets servo angle
  do {
    positionofservo += 1;
    armServo.write(positionofservo); // Keeps tightening servo arm until the block is grabbed
    delay(15);
  } while (positionofservo <= 100);
  
}

void IdentifyBlock(){
  
}

void DropOffBlock(){
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
  LeftMotor->setSpeed(100);
  RightMotor->setSpeed(100);
  delay(600);
  turnRight();
  String path = ConvertToLocalPath(GetPathToTarget(3, 12));
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength-1; i++){ //Loops through each direction until the block is reached
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
    LeftMotor->setSpeed(0);
    RightMotor->setSpeed(0);

    detachInterrupt(digitalPinToInterrupt(leftjunctionsensorPin)); //interrupts triggered by front line sensors to stop turning
    detachInterrupt(digitalPinToInterrupt(rightjunctionsensorPin));
    }
  // Deals with the block and returns to the start before generating the next path
  }

void ReturnToDepo(){
  
}

// ############################# MAIN LOOP ########################

void loop(){
//  //String path[] = generatePath(); //gets a list of directions
  attachInterrupt(digitalPinToInterrupt(pushButton),SwitchButtonState,RISING);
  if (buttonPressed) { 
    //String path[] = {"L", "L", "F", "R", "L"};
    String path = ConvertToLocalPath(GetPathToTarget(0, 3));
    SetCurrentHeading(180);
    int directionsLength = path.length(); //path.size();
    for (int i = 0; i <= directionsLength-1; i++){ //Loops through each direction until the block is reached
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
    LeftMotor->setSpeed(0);
    RightMotor->setSpeed(0);

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