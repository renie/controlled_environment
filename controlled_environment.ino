#include "secrets.h"
#include "src/webcontrollers.h"
#include "src/controlled.h"
#include "src/sensors.h"
#include "src/wifiConnection.h"

void setup() {
  Serial.begin(BAUDRATE);
  analogSetAttenuation(ADC_11db);


  Sensors::initializePins();
  Sensors::begin();


  Controlled::initializePins();


  WifiConnection::turnOn();  
  Serial.print("Connecting WiFi");  
  while (!WifiConnection::isOn()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  String ip = WifiConnection::getLocalIP();
  Serial.print("Wi-Fi connected: ");
  Serial.println(ip);


  WebControllers::start();

  Serial.println("");
  Serial.println("------------------");
  Serial.println("");
}

int secondsAfterLastSensorReading = 0;
void loop() {
  delay(1000);

  // commands to the web controllers are checked every second
  WebControllers::checkRequestsQueue();

  // sensors will report just after the amount of seconds
  // set on the environment variable
  if (secondsAfterLastSensorReading > SECONDS_BEFORE_NEXT_SENSOR_READING) {
    secondsAfterLastSensorReading = 0;
    Sensors::handleSensors();
    Serial.println("");
    Serial.println("------------------");
    Serial.println("");
  } else {
    secondsAfterLastSensorReading++;
  }
}
