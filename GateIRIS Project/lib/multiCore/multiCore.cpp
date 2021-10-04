// Inclusões
#include "multiCore.h"

// Funções
void setupMultiCore(BaseType_t core) { xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, core); }

void taskLoRa(void *pvParameters) // Nucleo 0, falta adicionar semaforo binario.
{
  while (1)
    runningLoRa(&gateway, &server);
}
