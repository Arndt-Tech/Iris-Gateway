#ifndef _EEPROM_H
#define _EEPROM_H

// EEPROM
  #include "EEPROM.h"

// EEPROM Address
  #define EEPROM_SIZE   256       // 256 bytes reservados para EEPROM
  
  #define addr_ssid_min   0       // Endereço mínimo da string SSID
  #define addr_ssid_max  30       // Endereço máximo da string SSID
  
  #define addr_pass_min  40       // Endereço mínimo da string PASSWORD
  #define addr_pass_max  70       // Endereço máximo da string PASSWORD

  #define addr_IDuser_min     80  // Endereço mínimo da string SSID
  #define addr_IDuser_max    110  // Endereço máximo da string SSID

  #define addr_IDstation_min 120  // Endereço mínimo da string PASSWORD
  #define addr_IDstation_max 150  // Endereço máximo da string PASSWORD

  #define addr_rstCause 160   // Endereços onde são guardados os números das últimas causas de reset.

  
// Strings sizes
  #define ID_USER_SIZE 40
  #define ID_STATION_SIZE 30
  #define SSID_SIZE 30
  #define PASSWORD_SIZE 30

// EEPROM
  bool verify_EEPROM(int addr_min);
  bool writeEEPROM_String(const char* string, int addr);
  bool clearEEPROM(char addr0, char addr1);
  String readEEPROM(int addr_min);
  void writeError(const char* err);
  String readError();


#endif
