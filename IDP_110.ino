#include <Adafruit_MotorShield.h>
#include <Servo.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// ######################################### SETUP #################################

int leftlinesensorPin = 12;
int rightlinesensorPin = 11; // Connect sensor to input pin 3
int leftjunctionsensorPin = 10;
int rightjunctionsensorPin = 13;
int crashswitchPin = 3; 

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
 pinMode(leftlinesensorPin, INPUT); // declare LED as output
 pinMode(rightlinesensorPin, INPUT); // declare Micro switch as input
 armServo.attach(9); // attaches the servo on pin 9 to the servo object
 pinMode(crashswitchPin, INPUT);
}

// ############################################ FUNCTIONS ###########################

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin);
  return Junct;
}

void PickUpBlock(){
  int positionofservo = 0;
  do {
    positionofservo += 1;
    armServo.write(positionofservo);
    delay(15);
  } while (digitalRead(crashswitchPin) == LOW);
  
}

void MoveToNextJunction(){
  do {
  int valLeft = digitalRead(leftlinesensorPin); // read left input value
 Serial.print(valLeft);
 RightMotor->run(BACKWARD);
    RightMotor->setSpeed(valLeft*200);
    delay(10);


 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.print(valRight);
    LeftMotor->run(FORWARD);
        LeftMotor->setSpeed(valRight*200);
        delay(10);
 delay(100);
  } while (JunctionSense == 0);
};

// ############################# MAIN LOOP ########################

void loop(){
 MoveToNextJunction();
};
