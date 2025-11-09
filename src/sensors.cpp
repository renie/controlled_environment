#include <Arduino.h>
#include "../secrets.h"
#include "./sensors.h"
#include "./airHumidityAndTemperature.h"
#include "./resistiveSoilMoisture.h"
#include "./lightSensor.h"
#include "./wifiConnection.h"
#include "./reportingAPI.h"

namespace Sensors {
  void initializePins() {
    AirHumidityAndTemperature::initializePins();
    ResistiveSoilMoisture::initializePins();
    LightSensor::initializePins();
  }

  void begin() {
    AirHumidityAndTemperature::begin();
  }

  static void printReadings(SensorReadings readings) {
    String lightsState = "Off";
    if (LightSensor::isOn(readings.ldr)) {
      lightsState = "On";
    }
    Serial.println("Humidity on sensor 1: " + String(readings.humidity1));
    Serial.println("Temperature on sensor 1: " + String(readings.temperature1));
    Serial.println("Humidity on sensor 2: " + String(readings.humidity2));
    Serial.println("Temperature on sensor 2: " + String(readings.temperature2));
    Serial.println("LDR: " + lightsState);
    Serial.println("Soil Moisture: " + String(readings.soilPercentage) + "% (" + String(readings.soil) + ")");
  }

  static bool areSensorReadingsValid(SensorReadings readings) {
    /**
    * TODO: Integrate AirReadings and SensorReadings in such a way
    * that it is not needed anymore to recreate an AirReadings struct
    * out of a SensorReadings struct every time a function that 
    * accepts AirReadings needs to be used.
    */
    AirReadings airReadings = {
      readings.humidity1,
      readings.temperature1,
      readings.humidity2,
      readings.temperature2
    };

    return (
      !AirHumidityAndTemperature::areReadingsValid(airReadings) || 
      !LightSensor::isReadingValid(readings.ldr)
    );
  }

  static void reportSensorDataToRemoteAPI() {
    if (WifiConnection::isOn()) {
      int httpResponseCode = ReportingAPI::sendSensorData();

      if (httpResponseCode >= 200 && httpResponseCode < 300) {
        Serial.println("Sensor readings just sent.");
      } else if (httpResponseCode >= 400 && httpResponseCode < 500) {
        Serial.print("Check whether the variable API_TO_REPORT_SENSES_URL, on .env file, is correct. Response code: ");
        Serial.println(httpResponseCode);
      } else if ((httpResponseCode >= 300 && httpResponseCode < 400) || httpResponseCode >= 500) {
        Serial.print("Remote server set in API_TO_REPORT_SENSES_URL, on .env file, has a problem. Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Remote server offline. Response code: ");
        Serial.println(httpResponseCode);
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
  }

  void handleSensors() {
    SensorReadings readings = getReadings();

    printReadings(readings);

    if (!areSensorReadingsValid(readings)) {
      Serial.println("There is a sensor with wrong readings.");
      Serial.println("Not sending wrong readings to the remote server.");
      return;
    }

    ReportingAPI::setData(readings);

    reportSensorDataToRemoteAPI();
  }

  bool isLightOn() {
    int ldr = LightSensor::read();
    return (isnan(ldr) && LightSensor::isOn(ldr));
  }

  SensorReadings getReadings() {
    AirReadings airReadings = AirHumidityAndTemperature::readAllValues();
    int ldrReading = LightSensor::read();
    int soilMoisture = ResistiveSoilMoisture::read();
    
    SensorReadings readings = {
      airReadings.humidityOnSensor1,
      airReadings.temperatureOnSensor1,
      airReadings.humidityOnSensor2,
      airReadings.temperatureOnSensor2,
      ldrReading,
      ResistiveSoilMoisture::getCurrentReading(),
      ResistiveSoilMoisture::getCurrentReadingInPercentage()
    };

    return readings;
  }
}
