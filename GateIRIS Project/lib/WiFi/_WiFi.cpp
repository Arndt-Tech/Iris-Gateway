// Inclusões
#include "_WiFi.h"

// Funções
bool connectWifi(networkWiFi *wifi)
{
  static unsigned long connectWifi = 0;
  int t = 1, min = 1;
  WiFi.begin(wifi->SSID.c_str(), wifi->PASSWORD.c_str());
  Serial.print("Conectando em: ");
  Serial.println(wifi->SSID.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis() - connectWifi) > 5000)
    {
      WiFi.begin(wifi->SSID.c_str(), wifi->PASSWORD.c_str());

      Serial.println("\nTentando conectar na rede");
      Serial.println(wifi->SSID);
      Serial.print("Tentativa: " + String(t / (min * 2)));

      t++;
      if (t > 12)
        return 0;
      connectWifi = millis();
    }
  }

  Serial.println();
  Serial.print("Conectado na rede: ");
  Serial.println(wifi->SSID);
  Serial.print("Endereço de IP é : ");
  Serial.println(WiFi.localIP());
  return 1;
}

void reconnectWiFi(networkWiFi *wifi) // TIMEOUT de 60 segundos
{
  static unsigned long tReconnectWifi = 0;
  int t = 1, min = 1;
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis() - tReconnectWifi) > 5000)
    {
      WiFi.begin(wifi->SSID.c_str(), wifi->PASSWORD.c_str());

      Serial.println("\nReeconectando na rede " + String(wifi->SSID));
      Serial.print("Tentativa: " + String(t / (min * 2)));
      t++;
      if (t > (12 * min))
        resetModule();
      tReconnectWifi = millis();
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
    return ("UNKNOWN_ERR");
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