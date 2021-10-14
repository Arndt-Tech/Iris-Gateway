#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "specialFunc.h"
#include "chipID.h"
#include "_EEPROM.h"
#include "Bluetooth.h"
#include "_WiFi.h"
#include "_Firebase.h"
#include "_LoRa.h"
#include "OLED.h"
#include "task.h"

// Definições
#define time(a) (uint64_t)(a * 1000000) // Tempo em segundos

// Funções
void configBegin(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb, networkLora *gtw);
void setupDataSystem(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb, networkLora *gtw);
void statusRefresh(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb);

#endif