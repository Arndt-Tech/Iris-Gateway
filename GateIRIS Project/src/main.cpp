#include "systemFunctions.h"

networkFirebase server;
networkLora gateway;
networkWiFi net;
//hw_timer_t *WDT = NULL;

void setup()
{
  configBegin(&gateway, &net, &server);
}

void loop()
{
 
  //timerWrite(WDT, 0); 
}
