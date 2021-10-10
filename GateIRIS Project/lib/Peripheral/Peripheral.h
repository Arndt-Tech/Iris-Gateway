#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "_EEPROM.h"
#include "specialFunc.h"

// Definições
#define resetTime(a) (uint8_t)a

// Funções
void resetClear();

#endif