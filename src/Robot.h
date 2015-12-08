#ifndef ROBOT_H
#define ROBOT_H
#include <stdlib.h>
#include "Arduino.h"
#include "pins.h"
#include "Encoder.h"
#include <Servo.h>
#include "LiquidCrystal.h"
#include "L3G.h"
#include "Wire.h"
#include "DriveTrain.h"
#include "Fan.h"
#include "FlameDetector.h"

class Robot{

	public:
		DriveTrain* drivetrain;
		Fan* fan;
		FlameDetector* flame;
		static Robot* getInstance();
		void initializeSubsystems();
	private:
		Robot();//cant be invoked
		static Robot* robot;


};

#endif
