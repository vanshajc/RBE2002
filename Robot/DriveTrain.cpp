#include "DriveTrain.h"

DriveTrain::DriveTrain(int leftMotorPin1, int leftMotorPin2,
   int rightMotorPin1, int rightMotorPin2, int leftEncoderPin1,
 int leftEncoderPin2, int rightEncoderPin1, int rightEncoderPin2) :
 LEFT_ENC_PIN1(leftEncoderPin1), LEFT_ENC_PIN2(leftEncoderPin2  ){}

void DriveTrain::initialize(){
}

 void drive(int left, int right){
   if (left < 0){
     analogWrite(leftMotor2,left);
     analogWrite(leftMotor1, 0);
   }
   else{
     analogWrite(leftMotor2, 0);
     analogWrite(leftMotor1,left);
   }

   if (right < 0){
     analogWrite(rightMotor2, right);
     analogWrite(rightMotor1, 0);
   }
   else{
     analogWrite(rightMotor1, right);
     analogWrite(rightMotor2, 0);
   }
 }

 void driveArcade(int forwardSpeed, int turnRate) {
   int leftSpeed = 255*forwardSpeed - 255*turnRate;
   int rightSpeed = 255*forwardSpeed + 255*turnRate;
   drive(leftSpeed, rightSpeed);
 }
