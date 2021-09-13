// Inclusões
#include "_EEPROM.h"

// Funções
bool verify_EEPROM(uint8_t addr_min)
{
  if (EEPROM.read(addr_min) != 255)
    return 1;
  else
    return 0;
}

bool clear_EEPROM(uint8_t addr0, uint8_t addr1)
{
  for (int i = addr0; i <= addr1; i++)
  {
    EEPROM.write(i, 255);
    EEPROM.commit();
  }
  return 1;
}

bool write_EEPROM(String str, uint8_t addr)
{
  int i = 0;
  for (; i < strlen(str.c_str()); i++)
    EEPROM.write(addr + i, str[i]);
  EEPROM.write(addr + i, '\0');
  EEPROM.commit();
  return 1;
}

String read_EEPROM(uint8_t addr) { return EEPROM.readString(addr); }