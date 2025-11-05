#ifndef ReportingAPI_H

#define ReportingAPI_H
#include <Arduino.h>
#include <ArduinoJson.h>

namespace ReportingAPI {
  extern StaticJsonDocument<200> dataToReport;

  int sendSensorData();
  String getCurrentDataToBeSent();
  String getLastReportedData();
  
  template <typename T>
  void setSensorData(const String& name, T value) {
    dataToReport[name] = value;
  }
}

#endif
