#ifndef Controlled_H

#define Controlled_H

namespace Controlled {
  void initializePins();
  void setFanSpeed(int percentage);
  int getFanSpeed();
  void setLightsState(bool state);
  bool getLightsState();
}

#endif
