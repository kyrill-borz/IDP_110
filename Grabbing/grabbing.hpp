/*
void grabBlock(Servo grabber,Servo arm){ //use writeMicroseconds to drive servo for a set time
    for(int angle = 1500; angle<=2000; angle += 10){
        grabber.writeMicroseconds(angle);
        delay(15);
    }
}

void liftArm(Servo grabber,Servo arm){ 
    for(int angle = 500; angle<= 1000;angle+=10){
        arm.writeMicroseconds(angle);
        delay(15);
    arm.writeMicroseconds(700);
}

void dropBlock(Servo grabber,Servo arm){
    for(int angle = 2000; angle>=1500; angle -= 10){
    grabber.writeMicroseconds(angle);
    delay(15);
    }
}

void lowerArm(Servo grabber,Servo arm){
    for(int angle = 2000; angle >= 1500; angle-=10){
    arm.write(angle);
    delay(15);
}
*/

void setServoAngle(Servo servo, int angle, int speed=10) { // pass the servo, the target angle, and the speed in degrees per second
    int currentAngle = servo.read();
    
    // set the servo angle
    if (angle < currentAngle) {
        for (int i = currentAngle; i >= angle; i--) {
            servo.write(i);
            delay((1/speed) * 1000);
        }
    }
    else if (angle > currentAngle) {
        for (int i = currentAngle; i <= angle; i++) {
            servo.write(i);
            delay((1/speed) * 1000);
        }
    }
}
