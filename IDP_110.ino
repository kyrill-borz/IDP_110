#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

int leftlinesensorPin = 12;
int rightlinesensorPin = 11; // Connect sensor to input pin 3
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

void setup() {
 Serial.begin(9600); // Init the serial port
 pinMode(leftlinesensorPin, INPUT); // declare LED as output
 pinMode(rightlinesensorPin, INPUT); // declare Micro switch as input

}
void loop(){
 int valLeft = digitalRead(leftlinesensorPin); // read left input value
 RightMotor->run(FORWARD);
    RightMotor->setSpeed(valLeft);

 int valRight = digitalRead(rightlinesensorPin); // read right input value
    LeftMotor->run(FORWARD);
        LeftMotor->setSpeed(valRight);
 delay(100);
}
