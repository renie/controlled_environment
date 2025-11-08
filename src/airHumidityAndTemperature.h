#ifndef AirHumidityAndTemperature_H
#define AirHumidityAndTemperature_H

struct AirReadings {
  float humidityOnSensor1;
  float temperatureOnSensor1;
  float humidityOnSensor2;
  float temperatureOnSensor2;
};

namespace AirHumidityAndTemperature {
  void initializePins();
  void begin();
  AirReadings readAllValues();
  bool areReadingsValid(AirReadings readings);
  void printReadings(AirReadings readings);
}

#endif
