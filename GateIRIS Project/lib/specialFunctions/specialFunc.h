#ifndef _SPECIALFUNCTIONS_H
#define _SPECIALFUNCTIONS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>

// Funções
void IRAM_ATTR resetModule();
void setupWatchdogTimer(uint64_t tm); // Núcleo 1

#endif