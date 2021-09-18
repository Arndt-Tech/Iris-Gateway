#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "OLED.h"
#include "Bluetooth.h"
#include "_EEPROM.h"
#include "_WiFi.h"
#include "_LoRa.h"
#include "multiCore.h"
#include "specialFunc.h"
#include "chipID.h"
#include "_Firebase.h"

// Funções
void configBegin();
void setupDataSystem(networkLora *gtw, networkWiFi *wifi, networkFirebase *fb);
void getWiFi(networkWiFi *wifi);
void getFirebase(networkLora *gtw, networkFirebase *fb);
void shift_vector(int cursor, int max, String *vector);
void stationSeeker(networkFirebase *fb);

#endif