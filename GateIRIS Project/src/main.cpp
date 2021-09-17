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

  stationSeeker(&server);
}

void loop()
{
  stationSeeker(&server);

  for (uint16_t i = 0; i < MAX_STATIONS; i++)
    Serial.println("Estacao " + String(i) + " --> " + server.STATION_ID[i]);
  Serial.println("");

  delay(500);
}