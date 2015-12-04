#include "Robot.h"
#include "Fan.h"

Fan::Fan(int servoPin, int flameSensorPin, int fanPin):servoPin(SERVO_PIN){}

void initialize() {
  servo.attach(SERVO_PIN);
}

int sweep(){
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
    temp1 = analogRead(flameSensorPin);
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

void setFan(int speed) {
  analogWrite(fanPin, speed);
}
