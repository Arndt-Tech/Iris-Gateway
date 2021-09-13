#ifndef _WIFI_H
#define _WIFI_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <WiFi.h>
#include "specialFunc.h"

// Struct's
typedef struct _wifi
{
  String SSID;
  String PASSWORD;
} networkWiFi;

// Funções
bool connectWifi(networkWiFi *wifi);
void reconnectWiFi(networkWiFi *wifi);
String wifiStatusDebug(wl_status_t wfStatus);
String readMac();

#endif