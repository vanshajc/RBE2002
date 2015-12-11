#include "pins.h"
#include "Encoder.h"
#include "PID_v1.h"
#include "LiquidCrystal.h"
#include <Servo.h>
#include <Wire.h>
#include <L3G.h>
//#include "IMU.h"

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

int dir = 90;
Point current;
Point prev;
double z;


//IMU imu;
typedef enum {
  wallFollowState,
  findCandleState,
  turnToCandleState,
  driveToCandleState,
  blowCandleState,
  END
} 
State;

typedef enum {
    wallFollowDefault,
    driveAtOutsideCorner,
    turnAtOutsideCorner,
    driveToEndOfCorner,
    turnAtInsideCorner
} WallFollowState;
WallFollowState wState;

int thresh = 500; // 450
int initialDifference = 0;
int initialDirection = 0;

Encoder le(leftEncoder1, leftEncoder2);
Encoder re(rightEncoder1, rightEncoder2);

double IRinput, IRoutput, IRsetpoint = 450;
// p was 0.005
double kP = 0.003, kI= 0, kD = 0.000;
PID IRPID(&IRinput, &IRoutput, &IRsetpoint, kP, kI, kD, DIRECT);

Servo servo;

State state = wallFollowState;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);


void setup() {
  initialDifference = le.read() + re.read();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  servo.attach(servoPin);
  pinMode(stopPin, INPUT_PULLUP);
  drive(0, 0);
  delay(500);
  IRPID.SetMode(AUTOMATIC);
  analogWrite(fan, LOW);
  calibrate();
  servo.write(100);

}

void loop() {
  //if (digitalRead(stopPin) == LOW)
  //kill();
  //updateCoordinates();
  initialDirection = gyro_z;
  updateCoordinates();
  if (millis() % 5 == 0){

    displayCoordinates();
  }
  //turnLeft();
  //delay(1000);
  runStateMachine();
  //Serial.println(analogRead(backRightIR));
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
      initialLeftPosition = le.read();
      initialRightPosition = re.read();
      drive(0, 0);
      delay(1000);
    }
    break;
  case findCandleState:
    if (abs(le.read() + re.read() - initialLeftPosition - initialRightPosition) < 8500){
      drive(-128, 128);
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
    
    if (abs(le.read() - leftEncPosition) >= 50 && abs(re.read() - rightEncPosition) >= 50){
      drive(128, -128);
    }else{
      drive(0, 0);
      resetEncoders();
      //delay(10000);
      state = driveToCandleState;
    }
    break;
  case driveToCandleState:
    //driveArcade(0.7, 0);
    forward();
    if (analogRead(frontIR) > 300){
      drive(0, 0);
      state = blowCandleState;
    }
    break;
  case blowCandleState:
    z = getCandleHeight();
    analogWrite(fan, 255);
    state = END;
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
  double angle = (le.read() + re.read())*90/4250.0;
  p.x = n*cos(angle*3.14/180);
  p.y = n*sin(angle*3.14/180);
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
  lcd.print("X = ");
  lcd.print((int)current.x);
  lcd.print(" Y = ");
  lcd.print((int)current.y);
  lcd.setCursor(0, 1);
  lcd.print("Z = ");
  lcd.print(z);

  //lcd.print(s);
}

// void followWall(){
//   double val = kP*(analogRead(frontRightIR) - thresh); // read ir value
//   IRinput = analogRead(frontRightIR);
//   double val2 = analogRead(frontIR);

//   if (analogRead(frontRightIR) < 100 && analogRead(backRightIR) < 100){
    
//     initialDifference = le.read() + re.read();
//     lcd.setCursor(0, 0);
//     lcd.print("Turning Right");
//     turnRight();
//     long t = millis();
//     initialDifference = le.read() + re.read();
//     while (millis() - t < 1600){
//       lcd.setCursor(0, 0);
//       lcd.print("Forward");
//       forward();
//     }
//     drive(0, 0);
//     return;
//   }
//   initialDifference = le.read() + re.read();
//   if (analogRead(frontRightIR) < 100){
//     lcd.setCursor(0, 0);
//     lcd.print("Forward");
//     forward();
//     return;
//   }
//   if (val2 > 200){
//     drive(0, 0);
//     turnLeft();
//   }
//   //val = constrain(val, -0.5, 0.5);
//   driveArcade(0.50, val);
// }
long currLeft, currRight;
long t;
void followWall(){
  double val = kP*(analogRead(frontRightIR) - thresh);
  double val2 = analogRead(frontIR);
  switch(wState){
    case wallFollowDefault:
      if (analogRead(frontRightIR) < 100){
        resetEncoders();
        wState = driveAtOutsideCorner;
      }
      else if (val2 > 200){
        currLeft = le.read();
        currRight = re.read();
        wState = turnAtInsideCorner;
      }
      driveArcade(0.5, val);
      break;
    case driveAtOutsideCorner:
      forward();
      if (analogRead(frontRightIR < 100) && analogRead(backRightIR) < 100){
        currLeft = le.read();
        currRight = re.read();
        wState = turnAtOutsideCorner;
      }
      break;
    case turnAtOutsideCorner:
      if (abs(currLeft + currRight - le.read() - re.read()) < 3550){
        drive(128, -128);
      }
      else{
        resetEncoders();
        currLeft = le.read();
        currRight = re.read();
        t = millis();
        wState = driveToEndOfCorner;
      }
      break;
    case driveToEndOfCorner:
      forward();
      if (millis() - t > 1500){
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

double getCandleHeight() {
  int minVal = 0, temp1, temp2 = 0;
  double kYOffset = 7.875;
  double kXOffset = 10;
  double kDegreesRotation = radians(100);
  int kTicks = 180;
  double kStartAngle = radians(90);
  //determine the index of the servo when it points
  //at the cand
  for(int i = 0; i < kTicks; i++) {
    servo.write(i);

    Serial.print(i);
    Serial.print("\t");
    Serial.println(temp1);
    delay(20);
    temp1 = analogRead(frontFlame);
    if(temp1 > minVal) {
      temp2 = i;
      minVal = temp1;
    }
  }
  servo.write(temp2);
  //remap the value and add the offsets
  double kSensorAngle = map(temp2, 0, kTicks -1, radians(45), radians(-15));
  double kSensorYOffset = tan(kSensorAngle)*kXOffset;
  double kSensorXOffset = cos(kSensorAngle);
  return kSensorAngle*180/3.14;
}

int prevValue = 0;
boolean hasDetectedFlame(){
  if (analogRead(sideFlameTop) > flameDetect ||
    analogRead(sideFlameMid) > flameDetect ||
    analogRead(sideFlameBot) > flameDetect){

    if (analogRead(sideFlameTop) > prevValue)
      prevValue = analogRead(sideFlameTop);

    if (analogRead(sideFlameTop) - prevValue <= -50)
      return true;

  }

  return false;
}


void turnLeft(){
  //calibrate();
  Serial.println("Turning");
  int currLeft = le.read();
  int currRight = re.read();
  initialDirection+=90;
  //while (abs(gyro_z - initialDirection) > 5){
  while(abs(currLeft + currRight - le.read() - re.read()) < 4250){
    calculate();
    //driveArcade(0.5, -1*((gyro_z - initialDirection))/90.0 * 0.5);
    //driveArcade(0, -1*(4250 - currLeft + currRight - le.read() - re.read())/4250.0 *0.5);
    //delay(5);
    drive(-128, 128);
    //drive(0, 0);

  }
  prev.x = le.read();
  prev.y = re.read();
  initialDifference = le.read() + re.read();
  drive(0, 0);
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

void turnRight(){
  int currLeft = le.read();
  int currRight = re.read();

  while (abs(currLeft + currRight - le.read() - re.read()) < 4250){
    //driveArcade(0, -1*(4250 - currLeft + currRight - le.read() - re.read())/2250.0);
    drive(128, -128);
  }
  initialDifference = le.read() + re.read();
  drive(0, 0);
}

void resetEncoders(){
  initialDifference = le.read() + re.read();

}



