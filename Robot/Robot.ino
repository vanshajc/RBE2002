// Robot.ino
// This is the main file.
// Van is amazing
//change from Morgan
#include "pins.h"
#include <Encoder.h>
#include "PID_v1.h"
#include "LiquidCrystal.h"
#include <Servo.h>
#include "Command.h"
#include <Robot.h>

Point current;
Point prev;

int thresh = 450;
int initialDifference = 0;

Encoder le(leftEncoder1, leftEncoder2);
Encoder re(rightEncoder1, rightEncoder2);

double IRinput, IRoutput, IRsetpoint;
double kP = 0.01, kI= 0, kD = 0.000;
PID IRPID(&IRinput, &IRoutput, &IRsetpoint, kP, kI, kD, DIRECT);

Servo servo;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);


void setup() {
  initialDifference = le.read() + re.read();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  servo.attach(servoPin);
  pinMode(stopPin, INPUT_PULLUP);

  IRsetpoint = 800;
  IRPID.SetMode(AUTOMATIC);

}

void loop() {
  if (digitalRead(stopPin) == LOW)
    kill();
//  Serial.print("Left encoder: ");
//  Serial.print(le.read());
//  Serial.print(" Right encoder: ");
//  Serial.println(re.read());
//  Serial.print("IR Value: ");
//  Serial.println(analogRead(frontIR));
  //updateCoordinates();
  updateCoordinates();
  if (millis() % 50 == 0){
    
    displayCoordinates();
  }
  followWall();
  //driveArcade(.5, 0);
  //sweep();
}

Point getDistanceTraveled(){
  Point p;
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

void updateCoordinates(){
  Point p = getDistanceTraveled();
  current.x += p.x;
  current.y += p.y;
}

void displayCoordinates(){
  lcd.setCursor(0, 0);
  char s[16];
  Serial.println(current.x);
  Serial.println(current.y);
  Serial.println("-----");
  sprintf(s, "X = %d, Y = %d", current.x, current.y);
  
  lcd.print("X = ");
  lcd.print(current.x);
  //lcd.setCursor(0, 1);
  //lcd.print("Y = ");
  //lcd.print(current.y);
  
  Serial.println(s);
  //lcd.print(s);
}

void followWall(){
  double val = analogRead(frontRightIR) - thresh; // read ir value
  
  double val2 = analogRead(frontIR) - thresh;
  //Serial.println(val2);
  
  
  IRinput = analogRead(frontRightIR);
  IRPID.Compute();
  //driveArcade(0.5, IRoutput);
  
//  lcd.setCursor(0,0);
//  lcd.print(IRoutput);
//  
//  Serial.println(IRoutput);
//  Serial.print("My value: ");
//  Serial.println(val*0.01);
  driveArcade(0.35, val*0.005);
  
  
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
