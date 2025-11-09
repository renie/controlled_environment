#ifndef SENSORS_H

#define SENSORS_H

struct SensorReadings {
  float humidity1;
  float temperature1;
  float humidity2;
  float temperature2;
  int ldr;
  int soil;
  int soilPercentage;
};

namespace Sensors {
  void initializePins();
  void begin();
  void handleSensors();
  SensorReadings getReadings();
  bool isLightOn();
}

#endif
