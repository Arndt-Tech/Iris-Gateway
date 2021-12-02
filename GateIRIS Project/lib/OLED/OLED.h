#pragma once

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Wire.h>
#include <SSD1306.h>
#include "pinout.h"
#include "fonts.h"
#include "images.h"
#include "icons.h"
#include "Bluetooth.h"
#include "_WiFi.h"
#include "_Firebase.h"
#include "_LoRa.h"

// Funções
void setupOLED();
void dataBar(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb, networkLora *gtw, bool commit, bool clear);
void runnigSystem(networkLora *gtw, bool commit, bool clear);

