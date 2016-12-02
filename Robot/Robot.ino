#include "pins.h"
#include "Encoder.h"
#include "PID_v1.h"
#include "LiquidCrystal.h"
#include <Servo.h>
#include <Wire.h>
#include <L3G.h>

L3G gyro;
float gyro_x; //gyro x val
float gyro_y; //gyro x val
float gyro_z; //gyro x val
float gyro_xold; //gyro cummulative x value
float gyro_yold; //gyro cummulative y value
float gyro_zold; //gyro cummulative z value
float gerrx; // Gyro x error
float gerry; // Gyro y error
float gerrz; // Gyro 7 error

Point current;
Point prev;
double z;


long currLeft, currRight;
long t;

/* State Machine for the main program. */
typedef enum {
  wallFollowState, // state in which the robot wall follows
  findCandleState, // rotates the robot until it finds the angle that points directly to the flame
  turnToCandleState, // turn the robot to the angle found
  driveToCandleState, // drive forward 
  turnRightToScan, // turn right to begin the scan to realign robot to flame
  turnLittleState, // turn a little for adjustment
  blowCandleState, // blow the candle
  turnRightToHome, 
  goForwardToWall,
  wallFollowHome,
  END
} 
State;

/* State Machine for Wall Following */
typedef enum {
    wallFollowDefault,
    driveAtOutsideCorner,
    turnAtOutsideCorner,
    driveToEndOfCorner,
    turnAtInsideCorner
} WallFollowState;


int threshWallFollow = 450; // 450
int initialDifference = 0;
int direction = 0;
boolean display = true;

/* Declare and initialize the encoders */
Encoder le(leftEncoder1, leftEncoder2);
Encoder re(rightEncoder1, rightEncoder2);

/* Proportional constant for the wall following portion */
double kPWallFollow = 0.003;

Servo servo;

State state = wallFollowState;
WallFollowState wState;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

/* 
* Holds the previous value read by the side flame sensor.
* This variable is only used for the hasDetectedFlame function
*/
int prevValue = 0;


void setup() {
  resetEncoders();
  pinMode(fan, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  servo.attach(servoPin);
  pinMode(stopPin, INPUT_PULLUP);
  digitalWrite(fan, LOW);
  digitalWrite(led, LOW);
  drive(0, 0);
  delay(500);
  calibrate();
  servo.write(100);


  //state = blowCandleState;
  //z = getCandleHeight();
}

void loop() {
  //if (digitalRead(stopPin) == LOW)
  //kill();
  direction = gyro_z;
  updateCoordinates();
  if (millis() % 5 == 0 && display){
    displayCoordinates();
  }
  //followWall();
  //digitalWrite(fan, HIGH);
  runStateMachine();
  //analogWrite(fan, 255);
}
long leftEncPosition;
long rightEncPosition;
int maxFlame = 0;
long initialLeftPosition;
long initialRightPosition;

void runStateMachine(){
  switch(state){
  case wallFollowState: 
    followWall(); 
    if (hasDetectedFlame()){
      state = findCandleState;
      digitalWrite(led, HIGH);
      initialLeftPosition = le.read();
      initialRightPosition = re.read();
      drive(0, 0);
      delay(1000);
    }
    break;
  case findCandleState:
    if (analogRead(frontIR) > 400){
      drive(0, 0);
      state = driveToCandleState;
    }
    else if (abs(le.read() + re.read() - initialLeftPosition - initialRightPosition) < 8500){
      drive(-100, 100);
      if (analogRead(frontFlame) > maxFlame){
      leftEncPosition = le.read();
      rightEncPosition = re.read();
      maxFlame = analogRead(frontFlame);
      }
    }
    else{
      drive(0, 0);
      state = turnToCandleState;
    }
    break;
  case turnToCandleState: 
    if (abs(le.read() - leftEncPosition) >= 10 && abs(re.read() - rightEncPosition) >= 10){
      drive(100, -100);
    }else{
      resetEncoders();
      currLeft = le.read();
      currRight = re.read();
      state = turnLittleState;
      //state = driveToCandleState;
    }
    break;
  case turnLittleState:
    if (abs(currLeft + currRight - le.read() - re.read()) < 650){
      drive(100, -100);
    }
    else{
      drive(0, 0);
      resetEncoders();
      //delay(10000);
      state = driveToCandleState;
    }
    break;
  case driveToCandleState:
   //driveArcade(0.7, 0);
     if (analogRead(frontIR) > 250){
        drive(0, 0);
        z = getCandleHeight();
        t = millis();
        display = false;
        double newX = current.x - 4*cos(radians(gyro_z));
        double newY = current.y - 4*sin(radians(gyro_z));
        displayActualCoordinates(newX, newY);
        // lcd.setCursor(0, 1);
        // lcd.print("X = ");
        // lcd.print(newX);
        // lcd.print(" Y = ");
        // lcd.print(newY);
        state = blowCandleState;
      }
    else if (le.read()  - leftEncPosition > 4000){
      drive(0, 0);
      resetEncoders();
      currLeft = le.read();
      currRight = re.read();
      state = turnRightToScan;
    }
    else{
      forward();
    } 
    break;
  case turnRightToScan:
    if (abs(currLeft + currRight - le.read() - re.read()) < 4250){
        drive(128, -128);
    }else{
      drive(0, 0);
      initialLeftPosition = le.read();
      initialRightPosition = re.read();
      state = findCandleState;
    }
    break;
  case blowCandleState:
    drive(0, 0);
    digitalWrite(fan, HIGH);
    if (millis() - t > 15000 && analogRead(frontFlame) < 100){
      digitalWrite(fan, LOW);
      digitalWrite(led, LOW);
      resetEncoders();
      currLeft = le.read();
      currRight = re.read();
      state = END;
    }
    break;

  case turnRightToHome:
    if (abs(currLeft + currRight - le.read() - re.read()) < 4250){
      drive(100, -100);
    }
    else{
      resetEncoders();
      state = goForwardToWall;
    }
    break;

  case goForwardToWall:
    forward();
    if (analogRead(frontIR) > 200){
      drive(0, 0);
      state = wallFollowHome;
    }
  
    break;
  case wallFollowHome:
    followWall();
    if (abs(current.x) <= 10 && abs(current.y) <= 10){
      drive(0, 0);
      state = END;
    }
  break;
  case END:
    drive(0, 0);
    break;
  }
}

Point getDistanceTraveled(){
  Point p;
  double n = ((le.read() - prev.x) - (re.read() - prev.y))/(3200*2) * 8.95;
  // incorporate angle here
  double angle = (le.read() + re.read())*90/410.0; // 90/4250
  p.x = n*cos(radians(angle));
  p.y = n*sin(radians(angle));
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
  displayActualCoordinates(current.x, current.y);
  //lcd.print(s);
}

void displayActualCoordinates(int x, int y){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("X = ");
  lcd.print((int) x);
  lcd.print(" Y = ");
  lcd.print((int) -1*y);
  lcd.setCursor(0, 1);
  lcd.print("Z = ");
  lcd.print(z);
}


void followWall(){
  double val = kPWallFollow*(analogRead(frontRightIR) - threshWallFollow);
  double val2 = analogRead(frontIR);
  switch(wState){
    case wallFollowDefault:
      if (analogRead(frontRightIR) < 100){
        resetEncoders();
        wState = driveAtOutsideCorner;
      }
      else if (val2 > 200){
        resetEncoders();
        currLeft = le.read();
        currRight = re.read();
        wState = turnAtInsideCorner;
      }
      //updateCoordinates();
      driveArcade(0.5, val);
      break;
    case driveAtOutsideCorner:
      forward();
      if (analogRead(frontRightIR < 100) && analogRead(backRightIR) < 100){
        //updateCoordinates();
        currLeft = le.read();
        currRight = re.read();
        wState = turnAtOutsideCorner;
      }
      break;
    case turnAtOutsideCorner:
      if (abs(currLeft + currRight - le.read() - re.read()) < 3750){
        drive(158, -158);
      }
      else{
        //updateCoordinates();
        resetEncoders();
        currLeft = le.read();
        currRight = re.read();
        t = millis();
        wState = driveToEndOfCorner;
      }
      break;
    case driveToEndOfCorner:
      forward();
      if (millis() - t > 1800){
        //updateCoordinates();
        resetEncoders();
        currLeft = le.read();
        currRight = re.read();
        if (analogRead(frontRightIR) > 200){
          wState = wallFollowDefault;
        }
        else 
          wState = turnAtOutsideCorner;
      }
      break;
    case turnAtInsideCorner:
      if (abs(currLeft + currRight - le.read() - re.read()) < 4250){
        drive(-128, 128);
      }
      else{
        //updateCoordinates();
        resetEncoders();
        currLeft = le.read();
        currRight = re.read();
        wState = wallFollowDefault;
      }
  }
}


void forward(){
  double turnRate = (le.read() + re.read() - initialDifference)*0.015;
  driveArcade(0.6, turnRate);
}

/**
 * Drive the robot.
 * @param forwardSpeed the speed of the motors for forward (-1 to 1)
 * @param turnRate the rate for which the robot needs to turn (-1 to 1)
 */
void driveArcade(double forwardSpeed, double turnRate){
  int leftSpeed = constrain(255*forwardSpeed - 255*turnRate, -255, 255);
  int rightSpeed = constrain(255*forwardSpeed + 255*turnRate, -255, 255);

  drive(leftSpeed, rightSpeed);
}

/**
 * Drive the two motors to the given speeds (-255 to 255).
 * @param left the duty cycle sent to the left motor
 * @param right the duty cycle sent to the right motor
 */
void drive(int left, int right){
  left = constrain(left, -255, 255);
  right = constrain(right, -255, 255);
  if (left < 0){
    analogWrite(leftMotor2,-1*left);
    analogWrite(leftMotor1, 0);
  }
  else{
    analogWrite(leftMotor2, 0);
    analogWrite(leftMotor1,left);
  }

  if (right < 0){
    analogWrite(rightMotor2, -1*right);
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

/**
* Blocking function that scans the servo from its lowest to highest range
* and finds the position where the front flame sensor detects the flame
* the strongest.
* @return the height in inches of the candle
*/
double getCandleHeight() {
  double maxVal = 0, maxReading, maxIndex = 0;
  double kYOffset = 7.875;
  double kXOffset = 6;
  double kDegreesRotation = radians(100);
  int kTicks = 180;
  // move the servo to the start to begin scan
  servo.write(0);
  delay(1000); 
  double kStartAngle = radians(90);
  //determine the index of the servo when it points
  //at the cand
  for(int i = 0; i < kTicks; i++) {
    servo.write(i);

    Serial.print(i);
    Serial.print("\t");
    Serial.println(maxReading);
    delay(20);
    maxReading = analogRead(frontFlame);
    if(maxReading > maxVal) { // if the maxVal is less than current reading
      maxIndex = i; // update max index
      maxVal = maxReading; // update max reading
    }
  }
  servo.write(maxIndex);
  double kSensorAngle = radians(10 + (maxIndex - 100)/3 );

  double kSensorYOffset = tan(kSensorAngle)*kXOffset;
  double kSensorXOffset = cos(kSensorAngle);
  return kYOffset + kSensorYOffset;
}



/*
* Returns true when the robot detects the flame, usually returns true,
* when robot is roughly perpendicular to the flame
* @return if the side flame sensor has detected the flame
*/
boolean hasDetectedFlame(){
  if (analogRead(sideFlameTop) > flameDetect){

    if (analogRead(sideFlameTop) > prevValue)
      prevValue = analogRead(sideFlameTop);

    if (analogRead(sideFlameTop) - prevValue <= -10)
      return true;

  }

  return false;
}


float G_Dt=0.005;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer=0;   //general purpose timer
long timer1=0;  

float G_gain=.00875; // gyros gain factor for 250deg/sec
void calibrate(){

  Wire.begin(); // i2c begin
  if (!gyro.init()) // gyro init
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1); 
  }
  timer=millis(); // init timer for first reading
  gyro.enableDefault(); // gyro init. default 250/deg/s
  delay(1000); // allow time for gyro to settle

  for(int i =0;i<100;i++){  // takes 100 samples of the gyro
    gyro.read();
    gerrx+=gyro.g.x;
    gerry+=gyro.g.y;
    gerrz+=gyro.g.z;
    delay(25);
  }

  gerrx = gerrx/100; // average reading to obtain an error/offset
  gerry = gerry/100;
  gerrz = gerrz/100;

  //  Serial.println(gerrx); // print error vals
  //  Serial.println(gerry);  
  //  Serial.println(gerrz);

}


void calculate(){
  if((millis()-timer)>=5)  // reads imu every 5ms
  {

    gyro.read(); // read gyro
    timer=millis(); //reset timer
    gyro_x=(float)(gyro.g.x-gerrx)*G_gain; // offset by error then multiply by gyro gain factor 
    gyro_y=(float)(gyro.g.y-gerry)*G_gain;
    gyro_z=(float)(gyro.g.z-gerrz)*G_gain;

    gyro_x = gyro_x*G_Dt; // Multiply the angular rate by the time interval
    gyro_y = gyro_y*G_Dt; 
    gyro_z = gyro_z*G_Dt;

    gyro_x +=gyro_xold; // add the displacment(rotation) to the cumulative displacment
    gyro_y += gyro_yold;
    gyro_z += gyro_zold;

    gyro_xold=gyro_x ; // Set the old gyro angle to the current gyro angle
    gyro_yold=gyro_y ;
    gyro_zold=gyro_z ;

  }


  if((millis()-timer1)>=1000)  // prints the gyro value once per second
  {
    timer1=millis();

    Serial.print("G ");
    Serial.print("X: ");
    Serial.print(gyro_x);
    Serial.print(" Y: ");
    Serial.print(gyro_y);
    Serial.print(" Z: ");
    Serial.println(gyro_z);
  }


}

// void turnRight(){
//   int currLeft = le.read();
//   int currRight = re.read();

//   while (abs(currLeft + currRight - le.read() - re.read()) < 4250){
//     //driveArcade(0, -1*(4250 - currLeft + currRight - le.read() - re.read())/2250.0);
//     drive(128, -128);
//   }
//   initialDifference = le.read() + re.read();
//   drive(0, 0);
// }

void resetEncoders(){
  initialDifference = le.read() + re.read();
}

