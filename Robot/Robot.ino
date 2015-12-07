// Robot.ino
// This is the main file.
// Van is amazing
//change from Morgan
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


Point current;
Point prev;
//IMU imu;
typedef enum {
  wallFollowState,
  turnToCandleState,
  driveToCandleState,
  blowCandleState,
  END
} State;

int thresh = 450;
int initialDifference = 0;

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
  servo.write(160);

}

void loop() {
  //if (digitalRead(stopPin) == LOW)
    //kill();
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
  //Serial.print("Value of flame sensor: ");
  Serial.print("Flame: ");
  Serial.println(analogRead(frontFlame));
  //turnLeft();
  runStateMachine();
  //runStateMachine();
  //analogWrite(fan, 128);
  //drive(-200, 75);
  //delay(1000);
}
void runStateMachine(){
  switch(state){
    case wallFollowState: followWall(); 
      if (hasDetectedFlame()){
        state = turnToCandleState;
        drive(0, 0);
        delay(1000);
      }
      break;
    case turnToCandleState: 
      drive(-180, 95);
      Serial.println(analogRead(frontFlame));
      if (abs(analogRead(frontFlame)) < 800){
        drive(0, 0);
        delay(1000);
        state = driveToCandleState;
      }
      break;
    case driveToCandleState:
      //driveArcade(0.7, 0);
      drive(128, 138);
      if (abs(analogRead(frontIR) - 300) < 10){
        drive(0, 0);
        state = blowCandleState;
      }
      break;
    case blowCandleState:
      getCandleHeight();
      analogWrite(fan, 255);
      state = END;
      break;
    case END:
      drive(0, 0);
      break;
  }
}


  //driveArcade(.5, 0);
  //sweep();


Point getDistanceTraveled(){
  Point p;
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
  // Serial.println(current.x);
  // Serial.println(current.y);
  // Serial.println("-----");
  // sprintf(s, "X = %d, Y = %d", current.x, current.y);

  lcd.print("X = ");
  lcd.print(current.x);
  //lcd.setCursor(0, 1);
  //lcd.print("Y = ");
  //lcd.print(current.y);

  //lcd.print(s);
}

void followWall(){
  double val = kP*(analogRead(frontRightIR) - thresh); // read ir value
  IRinput = analogRead(frontRightIR);
  double val2 = analogRead(frontIR) - thresh;
  //Serial.println(val2);
  if (analogRead(frontRightIR) < 100){
   //drive(255, 0);
   drive(220, 150);
   delay(100);
   return;
   
  }
  
  if (val2 > -200){
    drive(0, 0);
    turnLeft();
  }
  IRPID.Compute();
  lcd.print(val);
  driveArcade(0.55, val);
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

void sweep(){
//  for (int i = 90; i<180; i+=10){
//    servo.write(i);
//    analogWrite(fan, 255);
//    delay(100);
//  }

servo.write(0);
}

double getCandleHeight() {
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

int prevValue = 0;
boolean hasDetectedFlame(){
Serial.println(analogRead(sideFlameTop) - prevValue);
  if (analogRead(sideFlameTop) > flameThreshold ||
      analogRead(sideFlameMid) > flameThreshold ||
      analogRead(sideFlameBot) > flameThreshold){

    if (analogRead(sideFlameTop) > prevValue)
      prevValue = analogRead(sideFlameTop);

    if (analogRead(sideFlameTop) - prevValue <= -50)
      return true;

  }

  return false;
}

int dir = 90;
void turnLeft(){
  //calibrate();
  while (abs(gyro_z - dir) > 1){
//    Serial.print("Gyro value: ");
//    Serial.print(gyro_x);
//    Serial.print(", ");
//    Serial.print(gyro_y);
//    Serial.print(", ");
//    Serial.println(gyro_z);
    drive(-128, 128);
    //delay(5);
    //drive(0, 0);
    calculate();
 }
 dir +=90;
    Serial.print("Gyro value: ");
    Serial.print(gyro_x);
    Serial.print(", ");
    Serial.print(gyro_y);
    Serial.print(", ");
    Serial.println(gyro_z);
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

set;
}
