#ifndef _OLED_H
#define _OLED_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Wire.h>
#include <SSD1306.h>
#include "pinout.h"
#include "fonts.h"
#include "images.h"
#include "icons.h"
#include "ChipID.h"
#include "_LoRa.h"

// Struct's externas
extern networkLora gateway;

// Funções
void setupOLED();
void dataBar(String icon, uint8_t stations, bool commit, bool clear);
void runnigSystem(networkLora *gtw, bool commit, bool clear);

#endif