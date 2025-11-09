#ifndef ResistiveSoilMoisture_H
#define ResistiveSoilMoisture_H

namespace ResistiveSoilMoisture {
  void initializePins();
  int read();
  int getCurrentReading();
  int getCurrentReadingInPercentage();
}

#endif