// Inclusões
#include "task.h"

// Funções
void setupTasks()
{
  //xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", STACK(4096), NULL, PRIORITY(5), NULL, CORE(0));
  xTaskCreatePinnedToCore(taskOled, "taskOled", STACK(2048), NULL, PRIORITY(3), NULL, CORE(1));
  xTaskCreatePinnedToCore(taskSeekStation, "taskSeekStation", STACK(8096), NULL, PRIORITY(5), NULL, CORE(1));
  xTaskCreatePinnedToCore(taskStatusStation, "taskStatusStation", STACK(8096), NULL, PRIORITY(5), NULL, CORE(1));
}

void taskLoRa(void *pvParameters)
{ 
  while (1)
    runningLoRa(&gateway, &server);
}

void taskOled(void *pvParameters) 
{
  while (1)
  {
    dataBar("3", server.TOTAL_STATIONS, true, true);
    vTaskDelay(10);
  }
}

void taskSeekStation(void *pvParameters)
{
  while (1)
  {
    stationSeeker(&server);
    vTaskDelay(1);
  }
}

void taskStatusStation(void *pvParameters)
{
  while (1)
  {
    setStatus(&server);
    vTaskDelay(1);
  }
}