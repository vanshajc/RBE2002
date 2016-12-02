#include "Scheduler.h"

Scheduler* Scheduler::instance = NULL;
Scheduler::Scheduler(){}

Scheduler* Scheduler::getInstance(){
  if(instance == NULL){
    instance = new Scheduler();
  }
  return instance;
}

void Scheduler::addCommand(Command* c) {
  commands.add(c);
}

void Scheduler::addSequential(Command* c) {
  c->setParallel(false);
  commands.add(c);
}

void Scheduler::addParallel(Command* c) {
  c->setParallel(true);
  commands.add(c);
}

void Scheduler::Scheduler::run() {
	//Execute commands until there is a command that is not parallel
	int size = commands.size();

	Command* command;
	bool isFinished;
	int i = 0;
	while(i<size) {
		//Get the command
		command = commands.get(i);
		isFinished = command->cycle();
		//Remove the command if it is finished
		if(isFinished) {
			commands.remove(i);
			size = commands.size();
			if(command->getParallel()) {
				//don't increment, since just removed
			} else {
				//command was sequential, done with this loop
				break;
			}
		} else {
			if(command->getParallel()) {
				//Command is parallel, increment the pointer
				i++;
			} else {
				//Command is sequential, done with this loop
				break;
			}
		}
	}

}
