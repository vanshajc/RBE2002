#include "FlameDetector.h"
#include "pins.h"
FlameDetector::FlameDetector(){}
bool FlameDetector::flameDetected(){
  return (analogRead(kSideFlame) < detected);
}
