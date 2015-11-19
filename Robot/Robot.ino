// Robot.ino
// This is the main file.
// Van is amazing
//change from Morgan
#include "pins.h"
#include <Encoder.h>
#include <PID_v1.h>

#include "LiquidCrystal.h"
#include <Servo.h>



int thresh = 400;
int initialDifference = 0;

Encoder le(leftEncoder1, leftEncoder2);
Encoder re(rightEncoder1, rightEncoder2);

Servo servo;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);


void setup() {
  initialDifference = le.read() + re.read();
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  servo.attach(servoPin);
  pinMode(stopPin, INPUT_PULLUP);

}

void loop() {
  if (digitalRead(stopPin) == LOW)
    kill();
  Serial.print("Left encoder: ");
  Serial.print(le.read());
  Serial.print(" Right encoder: ");
  Serial.println(re.read());
  Serial.print("IR Value: ");
  Serial.println(analogRead(frontIR));
  lcd.setCursor(0, 0);
  lcd.print(analogRead(frontIR));
  followWall();

  //sweep();
}


void followWall(){
  double val = analogRead(frontIR) - thresh; // read ir value
  driveArcade(1, val*0.01);
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
  int leftSpeed = 255*forwardSpeed - 255*turnRate;
  int rightSpeed = 255*forwardSpeed + 255*turnRate;
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

void kill(){
 drive(0, 0);
 exit(0);
}

void sweep(){
//  for (int i = 90; i<180; i+=10){
//    servo.write(i);
//    analogWrite(fan, 255);
//    delay(100);
//  }

servo.write(0);
}
