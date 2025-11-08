#include <Arduino.h>
#include "../secrets.h"
#include "./lights.h"

namespace Lights {
  static bool state = false;

  void initializePins() {
    pinMode(LIGHTS_PIN, OUTPUT);
    digitalWrite(LIGHTS_PIN, state);
  }

  void setState(bool newState) {
    state = newState;
    digitalWrite(LIGHTS_PIN, state);
  }

  bool getState() {
    return state;
  }
}