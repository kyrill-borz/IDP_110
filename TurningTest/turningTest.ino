#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// ######################################### SETUP #################################

int leftTurn= 10; 
int rightTurn = 13;
int leftForward = 11;
int rightForward = 12;
int turningLeft;
int turningRight;

Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);


void setup() {
 
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

 pinMode(leftTurn, INPUT); //set line sensors to inputs
 pinMode(rightTurn, INPUT); 
 pinMode(leftForward, INPUT);
 pinMode(rightForward, INPUT);

 attachInterrupt(digitalPinToInterrupt(leftForward),stopRightTurn,FALLING); //interrupts triggered by front line sensors to stop turning
 attachInterrupt(digitalPinToInterrupt(rightForward),stopLeftTurn,FALLING);

}

void turnLeft(){ //adjust turning functions to match motor orientations
  turningLeft = 1;
  while(turningLeft == 1){
    RightMotor->run(FORWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(BACKWARD);
    LeftMotor->setSpeed(200);
  }
}

void turnRight(){
  turningRight = 1;
  while(turningLeft == 1){
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(200);
  }
}

void stopLeftTurn(){  //functions called by Interrupts
  turningLeft = 0;
}

void stopRightTurn(){
  turningRight = 0;
}
