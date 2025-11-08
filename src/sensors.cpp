#include <Arduino.h>
#include "../secrets.h"
#include "./sensors.h"
#include "./airHumidityAndTemperature.h"
#include "./lightSensor.h"
#include "./wifiConnection.h"
#include "./reportingAPI.h"

namespace Sensors {
  void initializePins() {
    AirHumidityAndTemperature::initializePins();
    pinMode(SOILMOISTURE_PIN, INPUT);
  }

  void begin() {
    AirHumidityAndTemperature::begin();
  }

  void handleSensors() {

    AirReadings airReadings = AirHumidityAndTemperature::readAllValues();
    int ldrReading = LightSensor::read();

    int soilMoisture = analogRead(SOILMOISTURE_PIN);

    // Using calibration values to map soil moisture in a value between 0-100 for usage with 100%
    int soilMoistureValue = map(soilMoisture, RES_SENSOR__SOILMOISTURE__VERYDRY, RES_SENSOR__SOILMOISTURE__VERYWET, 0, 100);
    soilMoistureValue = constrain(soilMoistureValue, 0, 100);


    // Show sensor readings on Serial Monitor
    AirHumidityAndTemperature::printReadings(airReadings);
    Serial.println("LDR: " + String(ldrReading));
    Serial.println("Soil Moisture: " + String(soilMoistureValue) + "% (" + String(soilMoisture) + ")");


    // Validating sensor readings
    if (!AirHumidityAndTemperature::areReadingsValid(airReadings) || 
      !LightSensor::isReadingValid(ldrReading)) {
      Serial.println("There is a sensor with wrong readings.");
      Serial.println("Not sending wrong readings to the remote server.");
      return;
    }


    // Sets the already validated sensor readings as
    // the data to be reported
    ReportingAPI::setSensorData("humidity1", airReadings.humidityOnSensor1);
    ReportingAPI::setSensorData("temperature1", airReadings.temperatureOnSensor1);
    ReportingAPI::setSensorData("humidity2", airReadings.humidityOnSensor2);
    ReportingAPI::setSensorData("temperature2", airReadings.temperatureOnSensor2);
    ReportingAPI::setSensorData("soilMoisture", soilMoistureValue);
    ReportingAPI::setSensorData("light", ldrReading);

  
    // Reports the data to remote server, if there is Wifi connection
    // and logs different messages for different returned 
    // HTTP response codes 
    if (WifiConnection::isOn()) {
      int httpResponseCode = ReportingAPI::sendSensorData();

      if (httpResponseCode > 0) {
        Serial.println("Sensor readings just sent.");
      } else {
        if (httpResponseCode == -1) {
          Serial.println("Remote server offline.");
        } else {
          Serial.print("Error on sending POST: ");
          Serial.println(httpResponseCode);
        }
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}
