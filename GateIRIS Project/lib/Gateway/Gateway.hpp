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
#include "Additional.hpp"

namespace gtw
{
  class Gateway
  {
  private:
    //cfg::Log m_log;
    //spc::SpecialFunctions m_spc;
    static add::Additional m_additional;
    static per::GPIO m_gpio;
    static com::BLE m_ble;
    static com::Wifi m_wifi;
    static com::FirebaseServer m_firebase;
    static com::Lora m_lora;

  private:
    void checkSystem();
    bool readData();
    void tryToEnter();
    void requestData();

  public:
    struct manage
    {
      static per::GPIO &GPIO();
      static com::BLE &Bluetooth();
      static com::Wifi &Wifi();
      static com::FirebaseServer &Firebase();
      static com::Lora &LoRa();
    } manage;

  public:
    static add::Additional &Additional();
    void begin();
  };
}