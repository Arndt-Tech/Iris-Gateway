#include "_WiFi.h"


//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupWifi()
{ 
  while ((verify_EEPROM(addr_ssid_min) && verify_EEPROM(addr_pass_min) && verify_EEPROM(addr_IDuser_min) && verify_EEPROM(addr_IDstation_min)))
  {
    Serial.println ("Há dados na EEPROM");
    SSID_STR       = readEEPROM(addr_ssid_min);
    PASSWORD_STR   = readEEPROM(addr_pass_min);
    USER_ID_STR    = readEEPROM(addr_IDuser_min);
    STATION_ID_STR = readEEPROM(addr_IDstation_min);
    Serial.println ("Dados lidos!");
    
    while (!connectWifi(SSID_STR, PASSWORD_STR))
    {
      Serial.println ("Impossível conectar na rede");
      Serial.println ("Reescreva o SSID e a SENHA");
      fnConnect = 0;
      bluetoothConfig();
      writeBT ("!");
      getWifi();
    }
    Serial.println ("IRIS foi conectada com sucesso!");
    writeBT ("&");

    if (!fnConnect)
    {
      Serial.println ("Dados salvos na EEPROM");
      writeEEPROM_String (SSID_STR.c_str(), addr_ssid_min);
      writeEEPROM_String (PASSWORD_STR.c_str(), addr_pass_min);
    }
    Serial.println ("Conectado na rede!");
    delay (1000);
    break;
  }

  while (!(verify_EEPROM(addr_ssid_min) && verify_EEPROM(addr_pass_min) && verify_EEPROM(addr_IDuser_min) && verify_EEPROM(addr_IDstation_min)))
  { 
    Serial.println ("Não há dados na EEPROM");
    bluetoothConfig();
    getIDs_Firebase();
    getWifi();
    
    while (!connectWifi(SSID_STR, PASSWORD_STR))
    {     
      Serial.println ("Impossível conectar na rede");
      Serial.println ("Reescreva o SSID e a SENHA");
      writeBT ("!");  // Não foi possível conectar no WIFI     
      getWifi();
    }
    writeBT ("&");
    
    Serial.println ("IRIS foi conectada com sucesso!");
    writeEEPROM_String (SSID_STR.c_str(), addr_ssid_min);
    writeEEPROM_String (PASSWORD_STR.c_str(), addr_pass_min);
    Serial.println ("Dados salvos na EEPROM!");
    
    delay (1000);
    break;
  }
  // Desabilita o BLE para economizar memória e energia
  BLEDevice::deinit(false);
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_bt_mem_release(ESP_BT_MODE_BTDM);
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
bool connectWifi(String ssid, String pass)
{
  unsigned long connectWifi, timeOUT;
  int t = 1;  
  WiFi.begin(ssid.c_str(), pass.c_str());                          
  Serial.print("Conectando em: ");
  Serial.println(ssid.c_str());
  while (WiFi.status() != WL_CONNECTED)
  { 
    if ((millis() - connectWifi) > 5000)
    {
      WiFi.begin(ssid.c_str(), pass.c_str());
      connectWifi = millis();
    }
    if ((millis() - timeOUT) > 10000)
    {
      timeOUT = millis();
      Serial.println ("\nTentando conectar na rede");
      Serial.println (ssid.c_str());
      Serial.print ("Tentativa: ");
      Serial.println (t);
      t++;
      if (t > 6)return 0;
    }
  }
  Serial.println();
  Serial.print("Conectado na rede: ");
  Serial.println(ssid.c_str());
  Serial.print("Endereço de IP é : ");
  Serial.println(WiFi.localIP());
  return 1;
}



String wifiStatusDebug (int wfStatus)
{
  switch (wfStatus)
  {
    case   0: return("WL_IDLE_STATUS");     
    case   1: return("WL_NO_SSID_AVAIL");   
    case   2: return("WL_SCAN_COMPLETED");  
    case   3: return("WL_CONNECTED");      
    case   4: return("WL_CONNECT_FAILED");  
    case   5: return("WL_CONNECTION_LOST"); 
    case   6: return("WL_DISCONNECTED");    
    case 255: return("WL_NO_SHIELD");       
  }
}



void reconnectWiFi(String ssid, String pass)      // TIMEOUT de 60 segundos
{
  unsigned long tReconnectWifi, timeOUT;
  int t = 1;
  while (WiFi.status() != WL_CONNECTED)
  { 
    if ((millis() - tReconnectWifi) > 5000)
    {
      WiFi.begin(ssid.c_str(), pass.c_str());
      tReconnectWifi = millis();
    }
    if ((millis()-tReconnectWifi) > 10000)
    {
      tReconnectWifi = millis();
      Serial.println ("\nReeconectando na rede " + String (ssid.c_str()));
      Serial.println (ssid.c_str()); 
      Serial.print ("Tentativa: ");
      Serial.println (t);
      t++;
      if (t > 6)
      {
        writeError ("1");
        resetModule();
      }
    }
  }
}



String read_macAddr()
{
  String mac = String (WiFi.macAddress());
  mac.remove(2, 1);
  mac.remove(4, 1);
  mac.remove(6, 1);
  mac.remove(8, 1);
  mac.remove(10, 1);
  return mac;
}
