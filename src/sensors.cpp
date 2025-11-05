#include <Arduino.h>
#include <DHT.h>
#include "../secrets.h"
#include "./sensors.h"
#include "./wifiConnection.h"
#include "./reportingAPI.h"

#define DHTTYPE DHT22

DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);

namespace Sensors {
  void initializePins() {
    pinMode(DHT1_PIN, INPUT);
    pinMode(DHT2_PIN, INPUT);
    pinMode(SOILMOISTURE_PIN, INPUT);
  }

  void begin() {
    dht1.begin();
    dht2.begin();
  }

  void handleSensors() {

    // Reading sensors
    float humidity1 = dht1.readHumidity();
    float temperature1 = dht1.readTemperature();

    float humidity2 = dht2.readHumidity();
    float temperature2 = dht2.readTemperature();

    int soilMoisture = analogRead(SOILMOISTURE_PIN);
    int ldrReading = analogRead(LDR_PIN);


    // Using calibration values to map soil moisture in a value between 0-100 for usage with 100%
    int soilMoistureValue = map(soilMoisture, RES_SENSOR__SOILMOISTURE__VERYDRY, RES_SENSOR__SOILMOISTURE__VERYWET, 0, 100);
    soilMoistureValue = constrain(soilMoistureValue, 0, 100);


    // Show sensor readings on Serial Monitor
    Serial.println("H1: " + String(humidity1));
    Serial.println("HT: " + String(temperature1));
    Serial.println("H2: " + String(humidity2));
    Serial.println("T2: " + String(temperature2));
    Serial.println("LDR: " + String(ldrReading));
    Serial.println("Soil Moisture: " + String(soilMoistureValue) + "% (" + String(soilMoisture) + ")");


    // Validating sensor readings
    if (isnan(humidity1) || isnan(temperature1) || isnan(humidity2) || isnan(temperature2) || isnan(ldrReading)) {
      Serial.println("There is a sensor with wrong readings.");
      Serial.println("Not sending wrong readings to the remote server.");
      return;
    }


    // Sets the already validated sensor readings as
    // the data to be reported
    ReportingAPI::setSensorData("humidity1", humidity1);
    ReportingAPI::setSensorData("temperature1", temperature1);
    ReportingAPI::setSensorData("humidity2", humidity2);
    ReportingAPI::setSensorData("temperature2", temperature2);
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
