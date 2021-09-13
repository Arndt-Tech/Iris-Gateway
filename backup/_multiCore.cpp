#include "_multiCore.h"



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void taskLoRa (void *pvParameters)
{
  while(1)
  {
    if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
    {
      send_LoRa_Message(String(valve));
      tLoRaSend = xTaskGetTickCount(); 
    }
    receive_LoRa_Message (LoRa.parsePacket());
    vTaskDelay(1);
  }
}
