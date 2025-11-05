#include <WebServer.h>
#include "./webcontrollers.h"
#include "./controlled.h"

namespace WebControllers {
  static WebServer server(80);
  
  void setFan(int PWMSpeed) {
    Controlled::setFanPWM(PWMSpeed);
    server.send(200, "text/plain", "Fan set to (" + String(PWMSpeed) + ")");
  }

  static void setFan0() { setFan(0); }
  static void setFan50() { setFan(127); }
  static void setFan100() { setFan(255); }

  static void addRoutes() {
    server.on("/fanoff", setFan0);
    server.on("/fanon", setFan100);
    server.on("/fanhalf", setFan50);
  }

  void start() {
    addRoutes();
    server.begin();
  }

  void checkRequestsQueue() {
    server.handleClient();
  }
}