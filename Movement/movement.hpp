#include <Adafruit_MotorShield.h>
#include "..\PinDefinitions\pindefinitions.h"
using namespace std;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

int turningLeft;
int turningRight;
int JunctionStatus = 0;

Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin); //Sees whether junction has been hit
  return Junct;
}

void turnLeft(){ //adjust turning functions to match motor orientations
  delay(100);
  turningLeft = 1;
  while(turningLeft == 1){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
  }
}

void turnRight(){ 
  delay(100);
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
    delay(10); // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.print(valRight);
    LeftMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*200);
        delay(10);
 delay(100);
  } while (JunctionStatus == 0); // stops when a junction is hit
};

