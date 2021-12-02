#pragma once

// 
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"
#include "Log.h"
#include "specialFunctions.h"

// 
#define resetTime(a) (uint8_t) a

/**
 * @brief Peripherals.
 * 
 */
namespace per
{
  class GPIO
  {
    public:
      static void checkReset();
  };
}


