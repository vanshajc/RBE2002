#include <stdlib.h>
#include "pins.h"
#include "Encoder/Encoder.h"
#include <Servo.h>
#include "LiquidCrystal.h"
class Robot{


	public:
		Robot();
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

		int thresh;
		int initialDifference;

		Servo servo;

		//LiquidCrystal lcd;


};