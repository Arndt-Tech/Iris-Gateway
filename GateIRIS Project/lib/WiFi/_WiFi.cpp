// Inclusões
#include "_WiFi.h"

String com::Wifi::m_ssid;
String com::Wifi::m_password;
IPAddress com::Wifi::m_local_ip;
String com::Wifi::m_mac_addr;
IPAddress com::Wifi::m_subnet_mask;
IPAddress com::Wifi::m_gateway_ip;
IPAddress com::Wifi::m_dns_ip;
int8_t com::Wifi::m_signal;

// Funções
bool com::Wifi::connect()
{
  uint8_t t = 1;
  static unsigned long connectWifi = 0;
  WiFi.begin(m_ssid.c_str(), m_password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((xTaskGetTickCount() - connectWifi) > 5000)
    {
      WiFi.begin(m_ssid.c_str(), m_password.c_str());
      t++;
      if (t > 12)
        return 0;
      connectWifi = xTaskGetTickCount();
    }
  }
  m_local_ip = WiFi.localIP();
  m_mac_addr = WiFi.macAddress();
  m_subnet_mask = WiFi.subnetMask();
  m_gateway_ip = WiFi.gatewayIP();
  m_dns_ip = WiFi.dnsIP();
  return 1;
}

void com::Wifi::reconnect()
{
  uint8_t t = 1;
  static unsigned long tReconnectWifi = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((xTaskGetTickCount() - tReconnectWifi) > 5000)
    {
      WiFi.begin(m_ssid.c_str(), m_password.c_str());
      t++;
      if (t > 12)
        spc::SpecialFunctions::resetModule();
      tReconnectWifi = xTaskGetTickCount();
    }
  }
}

void com::Wifi::set::SSID(String ssid) { m_ssid = ssid; }

void com::Wifi::set::password(String password) { m_password = password; }

String com::Wifi::get::SSID() { return m_ssid; }

String com::Wifi::get::password() { return m_password; }

IPAddress com::Wifi::get::IP() { return m_local_ip; }

String com::Wifi::get::macAddress() { return m_mac_addr; }

IPAddress com::Wifi::get::subnetMask() { return m_subnet_mask; }

IPAddress com::Wifi::get::gatewayIP() { return m_gateway_ip; }

IPAddress com::Wifi::get::dnsIP() { return m_dns_ip; }

int8_t com::Wifi::get::signal() { return WiFi.RSSI(); }