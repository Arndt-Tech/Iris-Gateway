// Inclusões
#include "multiCore.h"

// Funções
void setupMultiCore(BaseType_t core) { xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, core); }

void taskLoRa(void *pvParameters) // Nucleo 0, falta adicionar semaforo binario.
{
  while (1)
  {
    static unsigned long tLoRaSend = 0;
    if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
    {
      send_LoRa_Message(&gateway, &server);
      tLoRaSend = xTaskGetTickCount();
    }
    gateway.packSize = LoRa.parsePacket();
    receive_LoRa_Message(&gateway, &server);
    gateway.stationCursor += 1;
    if (gateway.stationCursor >= server.TOTAL_STATIONS)
      gateway.stationCursor = 0;
    vTaskDelay(1);
  }
}
