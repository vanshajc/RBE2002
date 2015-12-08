#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Command.h"
#include "LinkedList.h"

class Scheduler {
  public:

    static Scheduler* getInstance();
    void addCommand(Command* c);
    void printCommands();
    void addParallel(Command* c);
    void addSequential(Command* c);
    void run();
    void end();
  private:
    Scheduler();
    static Scheduler* instance;
    LinkedList<Command* > commands;

};

#endif
