#include "secrets.h"
#include "src/webcontrollers.h"
#include "src/controlled.h"
#include "src/sensors.h"
#include "src/wifiConnection.h"

void setup() {
  Serial.print("Initializing board.");
  Serial.begin(BAUDRATE);
  analogSetAttenuation(ADC_11db);

  Serial.print("Initializing pins of controlled interfaces.");
  Controlled::initializePins();

  Serial.print("Initializing pin of sensors.");
  Sensors::initializePins();
  Sensors::begin();


  Serial.print("Connecting WiFi");
  WifiConnection::turnOn();  
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

int secondsAfterLastSensorReading = SECONDS_BEFORE_NEXT_SENSOR_READING;
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
