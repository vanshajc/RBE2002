#include "Command.h"
#include "Robot.h"
#include "pins.h"

class DriveToCandle : public Command{
	public:
		DriveToCandle(int _speed);
		void initialize();
		void execute();
		void end();
		bool isFinished();
	private:
		Robot* robot;
		bool initialized;
		bool running;
		bool parallel;
		int speed;
		static const int wallDetected = 200; //frontIR value when the robot is ready to turn
};
