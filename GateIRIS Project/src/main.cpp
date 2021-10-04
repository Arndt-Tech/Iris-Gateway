#include "generalTasks.h"

networkFirebase server;
networkLora gateway;
networkWiFi net;
hw_timer_t *WDT = NULL;

void setup()
{
  configBegin();
  setupOLED();
  setupDataSystem(&gateway, &net, &server);
  setupFirebase(&server);
  setupLoRa(&gateway);
  setupMultiCore(0);
  setupWatchdogTimer(time(10));
}

void loop()
{
  timerWrite(WDT, 0);
  stationSeeker(&server); 
  setStatus(&server); 
}
