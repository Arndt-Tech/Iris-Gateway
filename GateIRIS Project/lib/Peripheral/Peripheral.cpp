// 
#include "Peripheral.h"

// Funções
void per::GPIO::checkReset()
{
  uint8_t counter = 0;
  while (digitalRead(resetEEPROM))
  {
    vTaskDelay(1000);
    counter++;
    if (counter >= resetTime(5))
    {
      cfg::Log::clear(0, EEPROM_SIZE);
      spc::SpecialFunctions::resetModule();
    }
  }
}