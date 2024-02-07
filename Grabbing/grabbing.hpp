
void grabBlock(Servo grabber,Servo arm){ //use writeMicroseconds to drive servo for a set time
    for(int angle = 1500; angle<=2000; angle += 10){
        grabber.writeMicroseconds(angle);
        delay(15);
    }
}

void liftArm(Servo grabber,Servo arm){ 
    for(int angle = 1500; angle<= 2000;angle+=10){
        arm.writeMicroseconds(angle);
        delay(15);
    }

}

void dropBlock(Servo grabber,Servo arm){
    for(int angle = 2000; angle>=1500; angle -= 10){
    grabber.writeMicroseconds(angle);
    delay(15);
    }
}

void lowerArm(Servo grabber,Servo arm){
    for(int angle = 2000; angle >= 1500; angle-=10){
    arm.writeMicroseconds(angle);
    delay(15);
    }
}
