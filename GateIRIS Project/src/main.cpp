#include "systemFunctions.h"

networkBluetooth BLE;
networkWiFi net;
networkFirebase server;
networkLora gateway;
//hw_timer_t *WDT = NULL;

void setup() { configBegin(&BLE, &net, &server, &gateway); }

void loop() {} //timerWrite(WDT, 0);
