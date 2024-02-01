#ifndef MOVEMENT_H
#define MOVEMENT_H
#include <Arduino.h>
#include<Adafruit_MotorShield.h>

extern Adafruit_MotorShield AMFS;
extern Adafruit_DCMotor LeftMotor;
extern Adafruit_DCMotor RightMotor;
extern int turningLeft;
extern int turningRight;
extern int JunctionStatus;

void turnLeft();
void turnRight();
void MoveToNextJunction();
int JunctionSense();
void stopLeftTurn();
void stopRightTurn();

#endif
