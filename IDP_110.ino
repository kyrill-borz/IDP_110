#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

int leftlinesensorPin = 12;
int rightlinesensorPin = 11; // Connect sensor to input pin 3
 
 Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
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

int JunctionSense(){
  return 0;
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

void loop(){
 MoveToNextJunction();
};
