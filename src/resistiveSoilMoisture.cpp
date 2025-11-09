#include <Arduino.h>
#include "../secrets.h"
#include "./resistiveSoilMoisture.h"

namespace ResistiveSoilMoisture {
  static int currentReading = 0;
  static int currentReadingInPercentage = 0;

  void initializePins() {
    pinMode(SOILMOISTURE_PIN, INPUT);
  }

  int read() {
    currentReading = analogRead(SOILMOISTURE_PIN);
    return getCurrentReadingInPercentage();
  }

  int getCurrentReading() {
    return currentReading;
  }
  
  int getCurrentReadingInPercentage() {
    int soilMoistureValue = map(currentReading, RES_SENSOR__SOILMOISTURE__VERYDRY, RES_SENSOR__SOILMOISTURE__VERYWET, 0, 100);
    currentReadingInPercentage = constrain(soilMoistureValue, 0, 100);
    return currentReadingInPercentage;
  }
}