#ifndef _EEPROM_H
#define _EEPROM_H

// Incluões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "EEPROM.h"

// EEPROM Address
#define EEPROM_SIZE 255 // 256 bytes reservados para EEPROM

// Local chipID
#define loChID_min 0
#define loChID_max 10

// Sender chipID
#define chID_min 15
#define chID_max 25

// SSID
#define ssid_min 30
#define ssid_max 70

// Password
#define password_min 80
#define password_max 130

// User ID
#define UserID_min 140
#define UserID_max 170

// Funções
bool verify_EEPROM(uint8_t addr_min);
bool write_EEPROM(String str, uint8_t addr);
bool clear_EEPROM(uint8_t addr0, uint8_t addr1);
String read_EEPROM(uint8_t addr_min);

#endif