// Robot.ino
// This is the main file. 
// Van is amazing
//change from Morgan
#include "pins.h"
#include <Encoder.h>


int initialDifference = 0;

Encoder le(leftEncoder1, leftEncoder2);
Encoder re(rightEncoder1, rightEncoder2);

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);


void setup() {
  initialDifference = le.read() + re.read();
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  digitalWrite(leftMotor1, 0);
  digitalWrite(leftMotor2, 0);
  digitalWrite(rightMotor1, 0);
  digitalWrite(rightMotor2, 0);

}

void loop() {
  //drive(255, 255);
  Serial.print("Left encoder: ");
  Serial.print(le.read());
  Serial.print(" Right encoder: ");
  Serial.println(re.read());
  forward();
}



void forward(){
  double turnRate = (le.read() + re.read() - initialDifference)*0.015;
  driveArcade(1, turnRate);
}

/**
* Drive the robot.
* @param forwardSpeed the speed of the motors for forward (-1 to 1)
* @param turnRate the rate for which the robot needs to turn (-1 to 1)
*/
void driveArcade(double forwardSpeed, double turnRate){
  int leftSpeed = 128*forwardSpeed - 128*turnRate;
  int rightSpeed = 128*forwardSpeed + 128*turnRate;
  drive(leftSpeed, rightSpeed);
}

/**
* Drive the two motors to the given speeds (-255 to 255).
* @param left the duty cycle sent to the left motor
* @param right the duty cycle sent to the right motor
*/
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

