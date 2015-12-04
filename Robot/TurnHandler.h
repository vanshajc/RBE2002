#include "Command.h"
#include "Robot.h"
#include "StackArray/StackArray.h"

class TurnHandler : public Command{
	public:
		TurnHandler();

		void execute();
		bool isFinished();
		StackArray<bool> turns;
};
