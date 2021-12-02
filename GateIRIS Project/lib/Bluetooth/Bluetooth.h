#pragma once

// Inclusões de bibliotecas
#include <Arduino.h>
#include <FreeRTOS.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "esp_err.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_main.h"
#include "pinout.h"
#include "debug.h"
#include "specialFunctions.h"
#include "_LoRa.h"
#include "_WiFi.h"

// ID's
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"           // ID serviço
#define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8" // ID de característica TX
#define CHARACTERISTIC_UUID_RX "808ccec4-d862-11eb-b8bc-0242ac130003" // ID de característica RX

// Senhas para protocolo de segurança
#define requestClientAppBT "tSdcxhoLVYj7oMT500&5"  // Recebe do app
#define passwordClientAppBT "KQ.?2qf01X|,JZRq8!}," // Envia para o app

// Flag's
#define SYNC_FLAG "@"
#define APP_SENDS_USERID "/"
#define APP_SENDS_SSID "~"
#define APP_SENDS_PASSWORD "^"
#define CONNECTION_ERROR "!"
#define SUCCESSFULLY_CONNECTED "&"

/**
 * @brief Communication
 * 
 */
namespace com
{
  class BLE
  {
  private:
    static BLEServer *m_serverBT;
    static BLECharacteristic *m_characteristic_TX;
    static BLECharacteristic *m_characteristic_RX;
    uint32_t m_BT_rxData;
    static uint8_t m_repeatDataFilter;
    static String m_data;
    static uint8_t m_connected;

  protected:
    static void callback();
    static void setConnectionStatus(uint8_t status);

  private:
    void config();
    void waiting();
    void waitingSYNC();
    void waitingRequest();
    bool getRequest();
    void sendRequest();
    void firebaseID();

  public:
    void begin();
    static String read();
    static String write(String data);
    static void refresh();
    void disable();
    static uint8_t getConnectionStatus();
    esp_bt_controller_status_t getStatus();
    void wifi();
  };
}