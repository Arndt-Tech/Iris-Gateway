#include <Arduino.h>
#include <FreeRTOS.h>
#include "configBegin.h"
#include "OLED.h"
#include "generalTasks.h"
#include "Bluetooth.h"
#include "_EEPROM.h"
#include "_WiFi.h"
#include "_LoRa.h"
#include "_Firebase.h"
#include "multiCore.h"
#include "specialFunc.h"
#include "chipID.h"

networkLora gateway;
networkWiFi net;
networkFirebase server;

void setup()
{
  configBegin();
  //clear_EEPROM(0, EEPROM_SIZE);
  setupOLED();
  setupDataSystem(&gateway, &net, &server);
  setupFirebase(&server);
  setupLoRa(&gateway);
  setupMultiCore(0);
}

void loop()
{
  readStation(&server);
  for (uint16_t i = 0; i < server.TOTAL_STATIONS; i++)
    Serial.printf("\nEstacao [%d] --> chipID: %s", i, server.STATION_ID[i].c_str());
  delay (100);
  delay(100);
  Serial.println("Heap Loop: " + String(xPortGetFreeHeapSize()));
}