#ifndef FAN_h
#define FAN_h

#include "Servo.h"


class Fan {
  public:
    Fan();
    void initialize();
    double sweep();
    void setFan(int speed);
    int getFlameSensor();


  private:
    Servo servo;

};
#endif
