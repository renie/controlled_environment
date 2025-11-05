#include <Arduino.h>
#include "../secrets.h"

namespace Controlled {
  void initializePins() {
    pinMode(FAN_PIN, OUTPUT);
  }

  void setFanPWM(int PWMValue) {
    analogWrite(FAN_PIN, PWMValue);
    Serial.println("Fan set to ("+String(PWMValue)+")");
    Serial.println("");
    Serial.println("------------------");
    Serial.println("");
  }
}
