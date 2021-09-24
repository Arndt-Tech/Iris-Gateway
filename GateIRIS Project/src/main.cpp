#include "generalTasks.h"

networkFirebase server;
networkLora gateway;
networkWiFi net;

void setup()
{
  configBegin();
  setupOLED();
  setupDataSystem(&gateway, &net, &server);
  setupFirebase(&server);
  setupLoRa(&gateway);
  //setupMultiCore(0); <----> Desabilitado
}

void loop()
{
  stationSeeker(&server);
  for (uint16_t i = 0; i < MAX_STATIONS; i++)
    Serial.println("Estacao " + String(i) + " --> " + *server.STATION_ID[i] + ": " + server.STATION_ID[i][ISON]);
  Serial.println("");
  runningLoRa(&gateway, &server);
}