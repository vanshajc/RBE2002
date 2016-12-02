#include "Command.h"
#include "Scheduler.h"

Command::Command() : initialized(false), parallel(false){}

// virtual void initialize();
// virtual void execute();
// virtual void end();
// virtual bool isFinished();
// virtual bool cycle();

void Command::initialize(){
  initialized = true;
}

void Command::execute(){

}

void Command::end(){
}

bool Command::isFinished(){
  return true;
}

bool Command::getParallel(){
  return parallel;
}

void Command::setParallel(bool p){
   parallel = p;
}

bool Command::getRunning(){
  return running;
}

bool Command::cycle() {
  bool finished = false;
  if(!initialized){
    initialize();
    initialized = true;
  } else if(isFinished()){
    finished = true;
    end();
  } else{
    execute();
  }
  return finished;
}
