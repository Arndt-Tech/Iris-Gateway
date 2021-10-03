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
  setupMultiCore(0);
}

void loop() { stationSeeker(&server); }