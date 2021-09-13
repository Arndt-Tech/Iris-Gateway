#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "configBegin.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "_EEPROM.h"
#include "_WiFi.h"
#include "_LoRa.h"
#include "multiCore.h"
#include "specialFunc.h"
#include "chipID.h"

// Funções
void configSystem(networkLora *gtw, networkWiFi *wifi);
void getWiFi(networkWiFi *wifi);
void getFirebase(networkLora *gtw);

#endif 