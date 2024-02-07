#include <Servo.h>

Servo grabber;
Servo arm;

void grabBlock(){ //use writeMicroseconds to drive servo for a set time
    grabber.writeMicroseconds(2000);
}

void liftArm(){ 
    arm.writeMicroseconds(1900);
}

void dropBlock(){
    grabber.writeMicroseconds(1000)
}

void lowerArm(){
    arm.writeMicroseconds(1000);
}
