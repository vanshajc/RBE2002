#include "Command.h"
#include "Robot.h"

class SweepServo : public Command{
	public:
		SweepServo();

		void execute();
		bool isFinished();
};
