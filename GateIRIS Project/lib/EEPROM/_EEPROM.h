#ifndef _EEPROM_H
#define _EEPROM_H

// Incluões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "EEPROM.h"

// EEPROM Address
#define EEPROM_SIZE 256 // 256 bytes reservados para EEPROM

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

// Station 1
#define stationID1_min 124
#define stationID1_max 134

// Station 2
#define stationID2_min 135
#define stationID2_max 145

// Station 3
#define stationID3_min 146
#define stationID3_max 156

// Station 4
#define stationID4_min 157
#define stationID4_max 167

// Station 5
#define stationID5_min 168
#define stationID5_max 178

// Funções
bool verify_EEPROM(uint8_t addr_min);
bool write_EEPROM(String str, uint8_t addr);
bool clear_EEPROM(uint8_t addr0, uint8_t addr1);
String read_EEPROM(uint8_t addr_min);

#endif