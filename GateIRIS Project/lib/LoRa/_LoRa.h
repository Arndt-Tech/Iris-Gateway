#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>
#include "pinout.h"
#include "debug.h"
#include "specialFunctions.h"
#include "_Firebase.h"

#define INTERVAL 2000
// LoRa shipping interval
#define BAND 433E6
// 433MHz Radio working frequency
#define SF 12
//
#define BW 250E3
//
#define CR 4
//
#define PL 6
//
#define SW 0x16
//
/*
  Config:
    Band -> 433MHz
    Spread Factor -> 12
    Bandwitch -> 250KHz
    Coding Rate -> 4
    Payload Lenght -> 10 bytes
    Preable Lenght -> 6
    Sync Word -> 0x16
  |----------------------------------|
  Data:
    Equivalent bitrate -> 366 bytes/s
    Time on air -> 561 ms
*/
//
#define SIZE_CORRECTION(a) (size_t)(a)

//
#define STD 0
#define DELETE 2

//
#define INTERVAL 2000
#define BAND 433E6

//
#define loraTmt 10000

/**
 * @brief Communication classes.
 * 
 */
namespace com
{
  class Lora;
}

/**
 * @brief Auxiliary classes.
 * 
 */
namespace aux
{
  class LoraPackage;
}

namespace aux
{
  class LoraPackage
  {
    friend class com::Lora;

  private:
    struct _snd
    {
      int32_t m_dest_addr;
      uint32_t m_local_addr;
      uint8_t m_is_on;
      uint8_t m_size;
    } static snd;

    struct _rcv
    {
      uint32_t m_receiver_addr, m_sender_addr;
      int32_t m_latitude, m_longitude;
      uint8_t m_humidity;
      int16_t m_temperature;
      uint8_t m_size, m_iterator;
      uint8_t m_valveStatus;
      int16_t m_signal;
    } static rcv;

  public:
    struct send
    {
      struct get
      {
        uint32_t destinationAddress() const;
        uint32_t localAddress() const;
        uint8_t valveStatus() const;
        uint8_t size() const;
      } get;
      struct set
      {
        void destinationAddress(uint32_t value);
        void localAddress(uint32_t value);
        void valveStatus(uint8_t status);
      } set;
    } send;

    struct receive
    {
      struct get
      {
        uint32_t receiveAddress() const;
        uint32_t senderAddress() const;
        int32_t latitude() const;
        int32_t longitude() const;
        int16_t temperature() const;
        uint8_t humidity() const;
        uint8_t stationNumber() const;
        uint8_t valve() const;
        int16_t signal() const;
        uint8_t size() const;
      } get;
    } receive;
  };
}

namespace com
{
  /**
   * @brief Communication class via Lora at the IRIS Gateway.
   * 
   */
  class Lora
  {
  private:
    static uint8_t m_selectedStation;

  private:
    static void organizeData();

  public:
    static aux::LoraPackage package;

  public:
    void begin();
    void checkTimeout();

  public:
    struct opr
    {
      static void readPackage();
      static void sendPackage();
      static void duplex();
    } operation;
  };
}
