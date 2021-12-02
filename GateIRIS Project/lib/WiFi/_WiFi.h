#pragma once

//
#include <Arduino.h>
#include <FreeRTOS.h>
#include <WiFi.h>
#include "pinout.h"
#include "debug.h"
#include "specialFunctions.h"

/**
 * @brief Communication
 * 
 */
namespace com
{
  class Wifi
  {
  private:
    static String m_ssid;
    static String m_password;
    static IPAddress m_local_ip;
    static String m_mac_addr;
    static IPAddress m_subnet_mask;
    static IPAddress m_gateway_ip;
    static IPAddress m_dns_ip;
    static int8_t m_signal;

  public:
    bool connect();
    void reconnect();
    struct set
    {
      static void SSID(String ssid);
      static void password(String password);
    } set;

    struct get
    {
      static String SSID();
      static String password();
      static IPAddress IP();
      static String macAddress();
      static IPAddress subnetMask();
      static IPAddress gatewayIP();
      static IPAddress dnsIP();
      static int8_t signal();
    } get;
  };
}
