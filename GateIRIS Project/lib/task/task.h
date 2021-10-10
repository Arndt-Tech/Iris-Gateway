#ifndef _MULTICORE_H
#define _MULTICORE_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "Peripheral.h"
#include "_LoRa.h"
#include "OLED.h"
#include "_Firebase.h"

// Definições
#define CORE(a) (BaseType_t)(a)
#define STACK(a) (uint32_t)(a)
#define PRIORITY(a) (UBaseType_t)(a)

// Struct's externas
extern networkFirebase server;
extern networkLora gateway;

// Funções
void setupTasks();

// Tasks
void taskReset(void *pvParameters);
void taskLoRa(void *pvParameters);
void taskLoRaTimeout(void *pvParameters);
void taskOled(void *pvParameters);
void taskSeekStation(void *pvParameters);
void taskStatusStation(void *pvParameters);

#endif