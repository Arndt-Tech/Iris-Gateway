#ifndef _EEPROM_H
#define _EEPROM_H

// Incluões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "EEPROM.h"

// EEPROM Address
#define EEPROM_SIZE 128 // 128 bytes reservados para EEPROM

// Local chipID
#define loChID_min 0
#define loChID_max 10

// SSID
#define ssid_min 11
#define ssid_max 41

// Password
#define password_min 42
#define password_max 92

// User ID
#define UserID_min 93
#define UserID_max 123

// Funções
bool verifyEEPROM(uint8_t addr_min);
bool writeEEPROM(uint32_t data, uint8_t addr);
bool writeStrEEPROM(String data, uint8_t addr);
bool clearEEPROM(uint8_t addr0, uint8_t addr1);
uint32_t readEEPROM(uint8_t addr);
String readStrEEPROM(uint8_t addr);

#endif