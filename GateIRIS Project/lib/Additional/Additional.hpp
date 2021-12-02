#pragma once
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"
#include "Log.h"
#include "specialFunctions.h"
#include "Peripheral.h"
#include "Bluetooth.h"
#include "_WiFi.h"
#include "_Firebase.h"
#include "_LoRa.h"

namespace add
{
  class Additional
  {
  public:
    void viewTokenInfo(token_info_t token);
    //wl_status_t
    String WiFiStatus();
  };
}
