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
    struct snd
    {
      static uint32_t m_dest_addr;
      static uint32_t m_local_addr;
      static uint8_t m_is_on;
      static uint8_t m_size;
    } snd;
    struct rcv
    {
      static uint32_t m_receiver_addr, m_sender_addr;
      static int32_t m_latitude, m_longitude;
      static uint8_t m_humidity, m_temperature;
      static uint8_t m_size, m_iterator;
      static int8_t m_signal;
    } rcv;

  public:
    struct send
    {
      struct get
      {
        static uint32_t destinationAddress();
        static uint32_t localAddress();
        static uint8_t valveStatus();
        static uint8_t size();
      } get;
      struct set
      {
        static void destinationAddress(uint32_t value);
        static void localAddress(uint32_t value);
        static void valveStatus(uint8_t status);
      } set;
    } send;

    struct receive
    {
      struct get
      {
        static uint32_t receiveAddress();
        static uint32_t senderAddress();
        static int32_t latitude();
        static int32_t longitude();
        static uint8_t temperature();
        static uint8_t humidity();
        static uint8_t stationNumber();
        static int8_t signal();
        static uint8_t size();
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
