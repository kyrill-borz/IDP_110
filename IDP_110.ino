#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// ######################################### SETUP #################################

int leftlinesensorPin = 12;
int rightlinesensorPin = 11; // Connect sensor to input pin 3
int leftjunctionsensorPin = 10;
int rightjunctionsensorPin = 13;
 
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);
  

void setup() {
 
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
 Serial.begin(9600); // Init the serial port
 pinMode(leftlinesensorPin, INPUT); // declare LED as output
 pinMode(rightlinesensorPin, INPUT); // declare Micro switch as input
}

// ############################################ FUNCTIONS ###########################

int JunctionSense(){
  int Junct = digitalRead(leftjunctionsensorPin) + digitalRead(rightjunctionsensorPin);
  return Junct;
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
