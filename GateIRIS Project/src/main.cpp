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

uint8_t part_st[5] = {stationID1_min, stationID2_min, stationID3_min, stationID4_min, stationID5_min};

void setup()
{
  configBegin();
  //clear_EEPROM(0, EEPROM_SIZE);
  setupOLED();
  setupDataSystem(&gateway, &net, &server);
  setupFirebase(&server);
  setupLoRa(&gateway);
  setupMultiCore(0);

  if (!readStation(&server))
    Serial.println("Nao encontrado");
  if (server.TOTAL_STATIONS > 0)
  {
    for (uint16_t i = 0; i < server.TOTAL_STATIONS; i++)
    {
      if (!verify_EEPROM(part_st[i]))
        write_EEPROM(server.STATION_ID[i], part_st[i]);
    }
  }
  else Serial.println("Nenhuma estacao");
}

void loop()
{
  //stationAnt = server.TOTAL_STATIONS;
  if (!readStation(&server))
    Serial.println("Nao encontrado");
  /*if (stationAnt > server.TOTAL_STATIONS)
  {
    server.STATION_ID[stationAnt];
  }*/
  
  for (uint16_t i = 0; i < server.TOTAL_STATIONS; i++)
    Serial.println("Estacao " + String(i) + " --> " + server.STATION_ID[i]);



  delay(500);
}