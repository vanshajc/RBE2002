#include <stdlib.h>
#include "Arduino.h"
#include "pins.h"
#include "Encoder/Encoder.h"
#include <Servo.h>
#include "LiquidCrystal.h"
#include "L3G.h"
#include "Wire.h"
class Robot{
	static Robot* robot;

	public:
		static Robot* getInstance();
		void initializeSubsystems();
		void setup();
		Point getDistanceTraveled();
		void updateCoordinates();
		void displayCoordinates();
		void followWall();
		void forward();
		void driveArcade(double forwardSpeed, double turnRate);
		void drive(int left, int right);
		Point current;
		Point prev;
		double getCandleHeight();
		bool checkForFlame();
		void initGyro();
		void turnAtWall();
		int thresh;
		int initialDifference;

	private:
		Robot();//cant be invoked

		Servo servo;
		L3G gyro;
		//LiquidCrystal lcd;


};
