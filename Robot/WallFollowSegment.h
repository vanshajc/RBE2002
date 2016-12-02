#include "Command.h"
#include "Robot.h"
#include "StackArray/StackArray.h"

class WallFollowSegment : public Command{
	public:
		StackArray<int> segments;
		WallFollowSegment();

		void execute();
		bool isFinished();

};
