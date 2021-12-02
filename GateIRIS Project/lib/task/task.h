#pragma once
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"
#include "Gateway.hpp"

extern gtw::Gateway gateway;

//
#define CORE(a) (BaseType_t)(a)
#define STACK(a) (uint32_t)(a)
#define PRIORITY(a) (UBaseType_t)(a)

//
void setupTasks();

//
void taskReset(void *pvParameters);
void taskLoRa(void *pvParameters);
void taskLoRaTimeout(void *pvParameters);
