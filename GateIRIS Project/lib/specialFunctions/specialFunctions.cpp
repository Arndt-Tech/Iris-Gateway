//
#include "specialFunctions.h"

/**
 * @brief Reset device.
 */
void spc::SpecialFunctions::resetModule() { esp_restart(); } 

/**
 * @brief Address assembly.
 * 
 * @param addr Address. 
 */
uint32_t spc::SpecialFunctions::asmAddr(uint8_t *addr)
{
  uint32_t newAddr = 0;
  newAddr |= addr[0];
  newAddr |= addr[1] << 8;
  newAddr |= addr[2] << 16;
  newAddr |= addr[3] << 24;
  return newAddr;
}

/**
 * @brief Controls timing functions in case of overflow.
 * 
 * @param ticks Time function.
 * @param currentTicks Variable that stores last ticks value.
 * @return Ticks.
 */
TickType_t spc::SpecialFunctions::ctrlTickCount(TickType_t ticks, TickType_t currentTicks)
{
  if (ticks > currentTicks)
    return ticks - currentTicks;
  else
    return ((pow(2, 8 * sizeof(ticks)) - 1) - currentTicks) + ticks;
}

/**
 * @brief Get the chip ID.
 * 
 * @return chip ID.
 */
uint32_t spc::SpecialFunctions::getChipID()
{
  uint32_t chipID = 0;
  for (int i = 0; i < 17; i = i + 8)
    chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  return chipID;
}