#ifndef _SPECIALFUNCTIONS_H
#define _SPECIALFUNCTIONS_H

  // Reset
  void IRAM_ATTR resetModule();
  
  // Watchdog Timer
  void setupWatchdogTimer();

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
// VARIÁVEIS 
  // Outros dados
  hw_timer_t *wTimer = NULL;

#endif
