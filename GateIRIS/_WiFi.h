#ifndef _WIFI_H
#define _WIFI_H

// WIFI
  #include <WiFi.h>

// WiFi
  void setupWifi();
  bool connectWifi(String ssid, String pass);
  void reconnectWiFi(String ssid, String pass);
  String wifiStatusDebug (int wfStatus);

#endif
