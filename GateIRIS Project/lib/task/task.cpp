// Inclusões
#include "task.h"

// Funções
void setupTasks()
{
  //xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", STACK(4096), NULL, PRIORITY(5), NULL, CORE(0));
  xTaskCreatePinnedToCore(taskLoRaTimeout, "taskLoRaTimeout", STACK(2048), NULL, PRIORITY(5), NULL, CORE(0));
  xTaskCreatePinnedToCore(taskOled, "taskOled", STACK(8192), NULL, PRIORITY(3), NULL, CORE(1));
}

// Tasks
void taskReset(void *pvParameters)
{
  while (1)
  {
    resetClear();
    vTaskDelay(1);
  }
}

void taskLoRa(void *pvParameters)
{
  while (1)
  {
    runningLoRa(&gateway, &server);
    vTaskDelay(1);
  }
}

void taskLoRaTimeout(void *pvParameters)
{
  while (1)
  {
    verify_LoRa_Timeout(&server);
    vTaskDelay(1);
  }
}

void taskOled(void *pvParameters)
{
  while (1)
  {
    dataBar(&BLE, &net, &server, &gateway, true, true);
    vTaskDelay(100);
  }
}