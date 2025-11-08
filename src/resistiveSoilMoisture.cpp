#include <Arduino.h>
#include "../secrets.h"
#include "./resistiveSoilMoisture.h"

namespace ResistiveSoilMoisture {
  void initializePins() {
    pinMode(SOILMOISTURE_PIN, INPUT);
  }

  int read() {
    int soilMoisture = analogRead(SOILMOISTURE_PIN);
    int soilMoistureValue = map(soilMoisture, RES_SENSOR__SOILMOISTURE__VERYDRY, RES_SENSOR__SOILMOISTURE__VERYWET, 0, 100);
    soilMoistureValue = constrain(soilMoistureValue, 0, 100);

    return soilMoistureValue;
  }
}