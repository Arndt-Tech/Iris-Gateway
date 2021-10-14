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
  IPAddress LOCAL_IP;
  String MAC_ADDR;
  IPAddress SUBNET_MASK;
  IPAddress GATEWAY_IP;
  IPAddress DNS_IP;
  int8_t SIGNAL;
} networkWiFi;

// Funções
bool connectWifi(networkWiFi *wifi);
void reconnectWiFi(networkWiFi *wifi);
String wifiStatusDebug(wl_status_t wfStatus);
String readMac();

#endif