#include "../secrets.h"
#include "./reportingAPI.h"
#include "./lightSensor.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char* apiUrl = API_TO_REPORT_SENSES_URL;

static String JSONtoString(StaticJsonDocument<200>& jsonToParse) {
  String jsonString;
  serializeJson(jsonToParse, jsonString);

  return jsonString;
}

namespace ReportingAPI {
  static HTTPClient request;
  static StaticJsonDocument<200> lastReportedData;
  StaticJsonDocument<200> dataToReport;

  String getCurrentDataToBeSent() {
    return JSONtoString(dataToReport);
  }

  String getLastReportedData() {
    return JSONtoString(lastReportedData);
  }

  String prepareJSONToBeSent() {
    return JSONtoString(dataToReport);
  }

  HTTPClient& prepareHTTPRequest() {
    request.end(); // just to be sure =)
    request.begin(apiUrl);
    request.addHeader("Content-Type", "application/json");

    return request;
  }

  void setData(SensorReadings sensorData) {
    dataToReport["humidity1"] = sensorData.humidity1;
    dataToReport["temperature1"] = sensorData.temperature1;
    dataToReport["humidity2"] = sensorData.humidity1;
    dataToReport["temperature2"] = sensorData.temperature1;
    dataToReport["soilMoisture"] = sensorData.soil;
    dataToReport["light"] = String(LightSensor::isOn(sensorData.ldr));
  }

  int sendSensorData() {
    HTTPClient& request = prepareHTTPRequest();
    String dataToPost = prepareJSONToBeSent();

    int response = request.POST(dataToPost);
    request.end();
    
    lastReportedData = dataToReport;
    dataToPost.clear();

    return response;
  }
}
