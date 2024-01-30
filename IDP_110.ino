#include <Adafruit_MotorShield.h>
#include <Servo.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// ######################################### SETUP #################################

// Calibrate input pins
int leftlinesensorPin = 12;
int rightlinesensorPin = 11; 
int leftjunctionsensorPin = 10;
int rightjunctionsensorPin = 13;
int crashswitchPin = 3; 
int pushButton = 7;

// Store variables for turning
int turningLeft;
int turningRight;

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
 armServo.attach(9); // attaches the servo on pin 9 to the servo object
}

// ############################################ FUNCTIONS ###########################

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin); //Sees whether junction has been hit
  return Junct;
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

void generatePath(){

};

void turnLeft(){ //adjust turning functions to match motor orientations
  turningLeft = 1;
  while(turningLeft == 1){ // defines a loop for turning to the left until interrupt is hit
    RightMotor->run(FORWARD);
    RightMotor->setSpeed(200);
    LeftMotor->run(BACKWARD);
    LeftMotor->setSpeed(200);
  }
}

void turnRight(){ 
  turningRight = 1;
  while(turningLeft == 1){ // defines a loop for turning to the right until interrupt is hit
    LeftMotor->run(FORWARD);
    LeftMotor->setSpeed(200);
    RightMotor->run(BACKWARD);
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
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
 Serial.print(valLeft);
 RightMotor->run(BACKWARD); // if left sensor is on the white line, turn the right wheel on
    RightMotor->setSpeed(valLeft*200);
    delay(10); // need to test if delay is necessary


 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.print(valRight);
    LeftMotor->run(FORWARD); // if left sensor is on the white line, turn the right wheel on
        LeftMotor->setSpeed(valRight*200);
        delay(10);
 delay(100);
  } while (JunctionSense == 0); // stops when a junction is hit
};

// ############################# MAIN LOOP ########################

void loop(){
 list String path = generatePath(); //gets a list of directions
 int directionsLength = path.size();
 for (int i = 0, i <= directionsLength, i++){ //Loops through each direction until the block is reached
  MoveToNextJunction(); // follows the line to next junction

  attachInterrupt(digitalPinToInterrupt(leftjunctionsensorPin),stopRightTurn,FALLING); //interrupts triggered by front line sensors to stop turning
  attachInterrupt(digitalPinToInterrupt(rightjunctionsensorPin),stopLeftTurn,FALLING);

  if (path[i] == "L"){ // decides what to do at each junction
    turnLeft();
  } else if (path[i] == "R"){
    turnRight();
  } else {
    cout << "continue straight"
  };

  detachInterrupt(digitalPinToInterrupt(leftForward)); //interrupts triggered by front line sensors to stop turning
  detachInterrupt(digitalPinToInterrupt(rightForward));
 }
 // Deals with the block and returns to the start before generating the next path
 FindBlock();
 PickUpBlock();
 IdentifyBlock();
 DropOffBlock();
 ReturnToDepo();
};
