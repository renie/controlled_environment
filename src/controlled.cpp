#include "./controlled.h"
#include "./fans.h"
#include "./lights.h"

namespace Controlled {
  void initializePins() {
    Fans::initializePins();
    Lights::initializePins();
  }

  void setFanSpeed(int percentage) {
    Fans::setSpeed(percentage);
  }

  int getFanSpeed() {
    return Fans::getSpeed();
  }

  void setLightsState(bool state) {
    Lights::setState(state);
  }

  bool getLightsState() {
    return Lights::getState();
  }
}
