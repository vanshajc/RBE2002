#include "Command.h"
#include "Robot.h"

class DriveToCandle : public Command{
	public:
		DriveToCandle();

		void execute();
		bool isFinished();
};
