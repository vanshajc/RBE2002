#ifndef FLAMEDETECTOR_H
#define FLAMEDETECTOR_H
#include "pins.h"
#include "LinkedList.h"
class FlameDetector {
   public:
     const static int detected = 800;
  FlameDetector();
 bool flameDetected();
  private:

};

#endif
