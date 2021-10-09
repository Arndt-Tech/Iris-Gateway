#include "generalTasks.h"

networkFirebase server;
networkLora gateway;
networkWiFi net;
//hw_timer_t *WDT = NULL;

void setup()
{
  configBegin();
  setupOLED();
  setupDataSystem(&gateway, &net, &server);
  setupFirebase(&server);
  setupLoRa(&gateway);
  setupMultiCore(0);
  //setupWatchdogTimer(time(20));  --> estabilizar
}

void loop()
{
 
  //timerWrite(WDT, 0);
  dataBar("3", server.TOTAL_STATIONS, true, true);
  stationSeeker(&server); 
  setStatus(&server); 
}
