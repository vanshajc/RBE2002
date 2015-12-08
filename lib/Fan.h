#ifndef FAN_h
#define FAN_h

#include "Robot.h"
#include "Servo.h"


class Fan {
  public:
    Fan(int servoPin, int flameSensorPin, int fanPin);
    void initialize();
    int sweep();
    void setFan(int speed);
    int getFlameSensor();


  private:
    Robot* robot;
    Servo servo;
    const int SERVO_PIN;
}
#endif
