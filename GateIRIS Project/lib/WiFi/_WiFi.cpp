// Inclusões
#include "_WiFi.h"

// Funções
bool connectWifi(networkWiFi *wifi)
{
  uint8_t t = 1;
  static unsigned long connectWifi = 0;
  WiFi.begin(wifi->SSID.c_str(), wifi->PASSWORD.c_str());
  Serial.print("Conectando em: ");
  Serial.println(wifi->SSID.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((xTaskGetTickCount() - connectWifi) > 5000)
    {
      WiFi.begin(wifi->SSID.c_str(), wifi->PASSWORD.c_str());

      Serial.println("\nTentando conectar na rede");
      Serial.println(wifi->SSID);
      Serial.println("Tentativa: " + String(t));
      t++;
      if (t > 12)
        return 0;
      connectWifi = xTaskGetTickCount();
    }
  }
  wifi->LOCAL_IP = WiFi.localIP();
  wifi->MAC_ADDR = WiFi.macAddress();
  wifi->SUBNET_MASK = WiFi.subnetMask();
  wifi->GATEWAY_IP = WiFi.gatewayIP();
  wifi->DNS_IP = WiFi.dnsIP();


  Serial.println();
  Serial.print("Conectado na rede: ");
  Serial.println(wifi->SSID);

  Serial.print("IP Address: ");
  Serial.println(wifi->LOCAL_IP);

  Serial.print("MAC Address: ");
  Serial.println(wifi->MAC_ADDR);

  Serial.print("Gateway IP: ");
  Serial.println(wifi->GATEWAY_IP);

  Serial.print("DNS IP: ");
  Serial.println(wifi->DNS_IP);

  return 1;
}

void reconnectWiFi(networkWiFi *wifi) // TIMEOUT de 60 segundos
{
  uint8_t t = 1;
  static unsigned long tReconnectWifi = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((xTaskGetTickCount() - tReconnectWifi) > 5000)
    {
      WiFi.begin(wifi->SSID.c_str(), wifi->PASSWORD.c_str());

      Serial.println("\nReeconectando na rede " + String(wifi->SSID));
      Serial.print("Tentativa: " + String(t));
      t++;
      if (t > 12)
        resetModule();
      tReconnectWifi = xTaskGetTickCount();
    }
  }
}

String wifiStatusDebug(wl_status_t wfStatus)
{
  switch (wfStatus)
  {
  case 0:
    return ("WL_IDLE_STATUS");
  case 1:
    return ("WL_NO_SSID_AVAIL");
  case 2:
    return ("WL_SCAN_COMPLETED");
  case 3:
    return ("WL_CONNECTED");
  case 4:
    return ("WL_CONNECT_FAILED");
  case 5:
    return ("WL_CONNECTION_LOST");
  case 6:
    return ("WL_DISCONNECTED");
  case 255:
    return ("WL_NO_SHIELD");
  default:
    return ("WL_UNKNOWN_ERR");
  }
}

String readMac()
{
  String mac = String(WiFi.macAddress());
  mac.remove(2, 1);
  mac.remove(4, 1);
  mac.remove(6, 1);
  mac.remove(8, 1);
  mac.remove(10, 1);
  return mac;
}