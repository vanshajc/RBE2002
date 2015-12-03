#include "Robot.h"



// Point current;
// Point prev;

// int thresh = 450;
// int initialDifference = 0;

 //Encoder le(leftEncoder1, leftEncoder2);
 //Encoder re(rightEncoder1, rightEncoder2);

// Servo servo;

Robot::Robot(){
	//thresh = 450;
	//initialDifference = 0;
	//le(leftEncoder1, leftEncoder2);
	//re(rightEncoder1, rightEncoder2);
	//lcd(40, 41, 42, 43, 44, 45);
}

void Robot::setup(){

	//initialDifference = le.read() + re.read();
  	Serial.begin(9600);
  	servo.attach(servoPin);
  	pinMode(stopPin, INPUT_PULLUP);
}

Point Robot::getDistanceTraveled(){
  Point p;
  //Serial.print(le.read());
  //Serial.println(re.read());
  //double n = ((le.read() - prev.x) - (re.read() - prev.y))/(3200*2) * 8.95;
  // incorporate angle here
  p.x = 0;
  p.y = 0;
  //prev.x = le.read();
  //prev.y = re.read();
 return p;
}

void Robot::updateCoordinates(){
  Point p = getDistanceTraveled();
  current.x += p.x;
  current.y += p.y;
}

void Robot::displayCoordinates(){
  // lcd.setCursor(0, 0);
  // char s[16];
  // Serial.println(current.x);
  // Serial.println(current.y);
  // Serial.println("-----");
  // sprintf(s, "X = %d, Y = %d", current.x, current.y);

  // lcd.print("X = ");
  // lcd.print(current.x);
  //lcd.setCursor(0, 1);
  //lcd.print("Y = ");
  //lcd.print(current.y);

  //Serial.println(s);
  //lcd.print(s);
}

void Robot::followWall(){
  double val = analogRead(frontRightIR) - thresh; // read ir value

  double val2 = analogRead(frontIR) - thresh;
//  Serial.println(val*0.01);
  driveArcade(0.35, val*0.005);


}

void Robot::forward(){
  driveArcade(1, 0);
}

/**
* Drive the robot.
* @param forwardSpeed the speed of the motors for forward (-1 to 1)
* @param turnRate the rate for which the robot needs to turn (-1 to 1)
*/
void Robot::driveArcade(double forwardSpeed, double turnRate){
  int leftSpeed = 255*forwardSpeed - 255*turnRate;
  int rightSpeed = 255*forwardSpeed + 255*turnRate;
  drive(leftSpeed, rightSpeed);
}

/**
* Drive the two motors to the given speeds (-255 to 255).
* @param left the duty cycle sent to the left motor
* @param right the duty cycle sent to the right motor
*/
void Robot::drive(int left, int right){
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
/**

* Get the height of the candle from a predetermined distance
* @return height of candle
*/
double Robot::getCandleHeight(){
  int minVal = 1023, temp1, temp2 = 0;
  double kYOffset = 7.875;
  double kXOffset = 2.5;
  double kDegreesRotation = radians(100);
  int kTicks = 180;
  double kStartAngle = radians(90);
  //determine the index of the servo when it points
  //at the candle
  for(int i = 0; i < kTicks; i++) {
    servo.write(i);
    temp1 = analogRead(kFrontFlame);
    Serial.print(i);
    Serial.print("\t");
    Serial.println(temp1);
    delay(20);
    if(temp1 < minVal) {
      temp2 = i;
      minVal = temp1;
    }
  }
  servo.write(temp2);
  //remap the value and add the offsets
  double kSensorAngle = map(temp2, 0, kTicks -1, radians(0), radians(kDegreesRotation));
  double kSensorYOffset = sin(kSensorAngle);
  double kSensorXOffset = cos(kSensorAngle);

  return kSensorYOffset+kYOffset;
}
