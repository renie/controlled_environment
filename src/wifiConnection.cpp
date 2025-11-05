#include "../secrets.h"
#include "./wifiConnection.h"
#include <Arduino.h>
#include <WiFi.h>

namespace WifiConnection {
  bool isOn() {
    return WiFi.status() == WL_CONNECTED;
  }
  void turnOn() {
    WiFi.begin(NETWORK_NAME, NETWORK_PASS);
  }
  String getLocalIP() {
    return WiFi.localIP().toString();
  }
}