// Inclusões
#include "configBegin.h"

// Funções
void configBegin()
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
}