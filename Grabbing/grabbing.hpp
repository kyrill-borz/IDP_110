#include <Servo.h>

void grabBlock(Servo grabber,Servo arm){ //use writeMicroseconds to drive servo for a set time
    grabber.writeMicroseconds(2000);
}

void liftArm(Servo grabber,Servo arm){ 
    arm.writeMicroseconds(1900);
}

void dropBlock(Servo grabber,Servo arm){
    grabber.writeMicroseconds(1000);
}

void lowerArm(Servo grabber,Servo arm){
    arm.writeMicroseconds(1000);
};
