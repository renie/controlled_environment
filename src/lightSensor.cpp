#include <Arduino.h>
#include "../secrets.h"
#include "./lightSensor.h"

namespace LightSensor {
  static int ldrReading;

  void initializePins() {
    pinMode(LDR_PIN, INPUT);
  }

  int read() {
    ldrReading = analogRead(LDR_PIN);
    return ldrReading;
  }

  bool isReadingValid(int reading) {
    return isnan(reading);
  }
}
