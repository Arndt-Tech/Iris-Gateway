// Inclusões
#include "specialFunc.h"

extern hw_timer_t *WDT;

// Funções
void IRAM_ATTR resetModule() { esp_restart(); } // Reinicia dispositivo

void setupWatchdogTimer(uint64_t tm)
{
  WDT = timerBegin(0x00, 80, true);
  timerAttachInterrupt(WDT, &resetModule, true);
  timerAlarmWrite(WDT, tm, true);
  timerAlarmEnable(WDT);
}