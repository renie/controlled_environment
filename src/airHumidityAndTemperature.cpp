#include <Arduino.h>
#include <DHT.h>
#include "../secrets.h"
#include "./airHumidityAndTemperature.h"

#define DHTTYPE DHT22

DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);

namespace AirHumidityAndTemperature {

  void initializePins() {
    pinMode(DHT1_PIN, INPUT);
    pinMode(DHT2_PIN, INPUT);
  }

  void begin() {
    dht1.begin();
    dht2.begin();
  }

  AirReadings readAllValues() {
    AirReadings readings = {
      dht1.readHumidity(),
      dht1.readTemperature(),
      dht2.readHumidity(),
      dht2.readTemperature()
    };

    return readings;
  }

  bool areReadingsValid(AirReadings readings) {
    return (isnan(readings.humidityOnSensor1) || 
      isnan(readings.temperatureOnSensor1) || 
      isnan(readings.humidityOnSensor2) || 
      isnan(readings.temperatureOnSensor2));
  }

  void printReadings(AirReadings readings) {
    Serial.println("Humidity on sensor 1: " + String(readings.humidityOnSensor1));
    Serial.println("Temperature on sensor 1: " + String(readings.temperatureOnSensor1));
    Serial.println("Humidity on sensor 2: " + String(readings.humidityOnSensor2));
    Serial.println("Temperature on sensor 2: " + String(readings.temperatureOnSensor2));
  }
}