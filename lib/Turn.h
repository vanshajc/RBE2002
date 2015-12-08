#include "Command.h"
#include "Robot.h"
#include "StackArray.h"
#include "L3G.h"

class Turn : public Command{
	public:
		Turn();

		void execute();
		bool isFinished();
	private:
		Robot* robot;
		StackArray<bool> turns;
		L3G gyro;
};
