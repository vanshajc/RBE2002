#include "DriveTrain.h"
#include "pins.h"

DriveTrain::DriveTrain() :le(kLeftEncoderA,kLeftEncoderB), re(kRightEncoderA,kRightEncoderB) {
  point.x = 0;
  point.y = 0;
}

void DriveTrain::initialize(){
}

/**
* Drive the two motors to the given speeds (-255 to 255).
* @param left the duty cycle sent to the left motor
* @param right the duty cycle sent to the right motor
*/
 void DriveTrain::drive(int left, int right){
   if (left < 0){
     analogWrite(kLeftMotorB,left);
     analogWrite(kLeftMotorA, 0);
   }
   else{
     analogWrite(kLeftMotorB, 0);
     analogWrite(kLeftMotorA,left);
   }

   if (right < 0){
     analogWrite(kLeftMotorB, right);
     analogWrite(kLeftMotorA, 0);
   }
   else{
     analogWrite(kLeftMotorA, right);
     analogWrite(kLeftMotorB, 0);
   }
 }

 /**
 * Drive the robot.
 * @param forwardSpeed the speed of the motors for forward (-1 to 1)
 * @param turnRate the rate for which the robot needs to turn (-1 to 1)
 */
 void DriveTrain::driveArcade(double forwardSpeed, double turnRate) {
   int leftSpeed = 255*forwardSpeed - 255*turnRate;
   int rightSpeed = 255*forwardSpeed + 255*turnRate;
   drive(leftSpeed, rightSpeed);
 }

void DriveTrain::turnLeft() {

}

 void DriveTrain::kill(){
 drive(0, 0);
 exit(0);
 }

 Point DriveTrain::getDistanceTraveled(){
 Point p;
 Point prev;
 prev.x = 0;
 prev.y = 0;
 Serial.print(le.read());
 Serial.println(re.read());
 double n = ((le.read() - prev.x) - (re.read() - prev.y))/(3200*2) * 8.95;
 // incorporate angle here
 p.x = n;
 p.y = 0;
 prev.x = le.read();
 prev.y = re.read();
 return p;
 }

 void DriveTrain::updateCoordinates(){
 Point p = getDistanceTraveled();
 point.x += p.x;
 point.y += p.y;
 }

 void DriveTrain::displayCoordinates(){
 char s[16];
 Serial.println(point.x);
 Serial.println(point.y);
 Serial.println("-----");
 sprintf(s, "X = %d, Y = %d", point.x, point.y);

 //lcd.print("X = ");
 //lcd.print(current.x);
 //lcd.setCursor(0, 1);
 //lcd.print("Y = ");
 //lcd.print(current.y);

 Serial.println(s);
 //lcd.print(s);
 }

 void DriveTrain::forward(){
   int initialDifference = 0;
 double turnRate = (le.read() + re.read() - initialDifference)*0.015;
 driveArcade(1, turnRate);
 }
