// 
#include "task.h"



// 
void setupTasks()
{
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", STACK(4096), NULL, PRIORITY(5), NULL, CORE(0));
  xTaskCreatePinnedToCore(taskLoRaTimeout, "taskLoRaTimeout", STACK(2048), NULL, PRIORITY(5), NULL, CORE(0));
}

// 
void taskReset(void *pvParameters)
{
  while (1)
  {
    gateway.manage.GPIO().checkReset();
    vTaskDelay(1);
  }
}

void taskLoRa(void *pvParameters)
{
  while (1)
  {
    gateway.manage.LoRa().operation.duplex();
    vTaskDelay(1);
  }
}

void taskLoRaTimeout(void *pvParameters)
{
  while (1)
  {
    gateway.manage.LoRa().checkTimeout();
    vTaskDelay(1);
  }
}