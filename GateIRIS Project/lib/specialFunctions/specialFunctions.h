#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"

namespace spc
{
  class SpecialFunctions
  {
  public:
    static void IRAM_ATTR resetModule();
    static uint32_t asmAddr(uint8_t *addr);
    static TickType_t ctrlTickCount(TickType_t ticks, TickType_t currentTicks);
    static uint32_t getChipID();
  };
}

