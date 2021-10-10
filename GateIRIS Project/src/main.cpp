#include "systemFunctions.h"

networkBluetooth BLE;
networkWiFi net;
networkFirebase server;
networkLora gateway;
//hw_timer_t *WDT = NULL;

void setup() { configBegin(&BLE, &net, &server, &gateway); }

void loop() 
{
  stationSeeker(&server);
  setStatus(&server);
  Serial.println("Heap: " + String(xPortGetFreeHeapSize()));
} //timerWrite(WDT, 0);
