#include "./controlled.h"
#include "./fans.h"

namespace Controlled {
  void initializePins() {
    Fans::initializePins();
  }

  void setFanSpeed(int percentage) {
    Fans::setSpeed(percentage);
  }

  int getFanSpeed() {
    Fans::getSpeed();
  }
}
