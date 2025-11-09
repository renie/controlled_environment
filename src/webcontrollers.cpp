#include <WebServer.h>
#include "./webcontrollers.h"
#include "./sensors.h"
#include "./controlled.h"

namespace WebControllers {
  static WebServer server(80);
  
  void setFan(int percent) {
    Controlled::setFanSpeed(percent);
    server.send(200, "text/plain", "Fan set to " + String(percent) + "%");
  }

  static void setFan0() { setFan(0); }
  static void setFan50() { setFan(50); }
  static void setFan100() { setFan(100); }

  static void getFanSpeed() { 
    server.send(200, "text/plain", String(Controlled::getFanSpeed()) + "%");
  }

  void setLights(bool state) {
    Controlled::setLightsState(state);
    server.send(200, "text/plain", "Lights set to: " + String(state) + "%");
  }

  static void setLightsOff() { setLights(false); }
  static void setLightsOn() { setLights(true); }

  static void getLightsState() {
    server.send(200, "text/plain", "State:" + String(Controlled::getLightsState()) + ",Sensor:" + String(Sensors::isLightOn()));
  }

  static void addRoutes() {
    server.on("/fanoff", setFan0);
    server.on("/fanon", setFan100);
    server.on("/fanhalf", setFan50);
    server.on("/fanspeed", getFanSpeed);
    server.on("/lightson", setLightsOn);
    server.on("/lightsoff", setLightsOff);
    server.on("/lightsstate", getLightsState);
  }

  void start() {
    addRoutes();
    server.begin();
  }

  void checkRequestsQueue() {
    server.handleClient();
  }
}