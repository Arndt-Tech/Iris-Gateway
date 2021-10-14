// Inclusões
#include "Peripheral.h"

// Funções
void resetClear()
{
  uint8_t counter = 0;
  while (digitalRead(resetEEPROM))
  {
    vTaskDelay(1000);
    counter++;
    if (counter >= resetTime(3))
    {
      clearEEPROM(0, EEPROM_SIZE);
      resetModule();
    }
  }
}