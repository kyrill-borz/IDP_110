#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <stdlib.h>
//#include <list>
using namespace std;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// ######################################### SETUP #################################

// Calibrate input pins
int leftlinesensorPin = 9;
int rightlinesensorPin = 8; 
int leftjunctionsensorPin = 10;
int rightjunctionsensorPin = 7;
int crashswitchPin = 3; 
int pushButton = 7;

// Store variables for turning
int turningLeft;
int turningRight;
int JunctionStatus = 0;
String directions[] = {"L","R"};

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

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin); //Sees whether junction has been hit
  return Junct;
}

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

//void generatePath(double* directions){
//  directions = {"L", "R", "L", "C"};
//};

void turnLeft(){ //adjust turning functions to match motor orientations
  turningLeft = 1;
  while(turningLeft == 1){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(BACKWARD);
    RightMotor->setSpeed(170);
    LeftMotor->run(BACKWARD);
    LeftMotor->setSpeed(200);
  }
}

void turnRight(){ 
  LeftMotor->run(FORWARD);
  RightMotor->run(BACKWARD);
  delay(200);
  turningRight = 1;
  while(turningRight == 1){ // defines a loop for turning to the right until interrupt is hit
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
    RightMotor->run(FORWARD);
    RightMotor->setSpeed(170);
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
    RightMotor->setSpeed(valLeft*170);
    delay(10); // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.print(valRight);
    LeftMotor->run(FORWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*200);
        delay(10);
 delay(100);
  } while (JunctionStatus == 0); // stops when a junction is hit
};

// ############################# MAIN LOOP ########################

void loop(){
//  //String path[] = generatePath(); //gets a list of directions
  String path[] = {"R", "R", "R", "R"};
  int directionsLength = 2; //path.size();
  for (int i = 0; i <= directionsLength; i++){ //Loops through each direction until the block is reached
    MoveToNextJunction(); // follows the line to next junction
    attachInterrupt(digitalPinToInterrupt(rightlinesensorPin),stopRightTurn,RISING); //interrupts triggered by front line sensors to stop turning
    attachInterrupt(digitalPinToInterrupt(leftlinesensorPin),stopLeftTurn,RISING);
   if (path[i] == "L"){ // decides what to do at each junction
     turnLeft();
   } else if (path[i] == "R"){
     turnRight();
   } else {
     //cout << "continue straight"
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
};
