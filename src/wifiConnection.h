#ifndef WifiConnection_H

#define WifiConnection_H
#include <Arduino.h>

namespace WifiConnection {
  bool isOn();
  void turnOn();
  String getLocalIP();
}

#endif
