#ifndef LightSensor_H
#define LightSensor_H

namespace LightSensor {
  void initializePins();
  int read();
  bool isReadingValid(int reading);
}

#endif