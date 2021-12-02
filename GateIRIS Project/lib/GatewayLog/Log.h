#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"
#include "EEPROM.h"

// EEPROM Address
#define EEPROM_SIZE 128

// Local chipID
#define loChID_min 0
#define loChID_max (ssid_min - 1)

// SSID
#define ssid_min 11
#define ssid_max (password_min - 1)

// Password
#define password_min 42
#define password_max (UserID_min - 1)

// User ID
#define UserID_min 93

/**
 * @brief Configuration
 * 
 */
namespace cfg
{
  class Log
  {
  public:
    static void begin();
    static bool check(uint8_t addr);
    static bool write(uint32_t data, uint8_t addr);
    static bool write(String data, uint8_t addr);
    static bool clear(uint8_t addr0, uint8_t addr1);
    static uint32_t read(uint8_t addr);
    static String reads(uint8_t addr);
  };
}