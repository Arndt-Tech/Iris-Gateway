// Inclusões
#include "generalTasks.h"

// Funções
void configSystem(networkLora *gtw, networkWiFi *wifi)
{
  bool inconsistent_data = 0;
  if ((verify_EEPROM(loChID_min) && verify_EEPROM(chID_min) && verify_EEPROM(ssid_min) && verify_EEPROM(password_min)))
  {
    Serial.println("Há dados na EEPROM");
    gtw->localAddr = atol(read_EEPROM(loChID_min).c_str());
    gtw->destAddr = atol(read_EEPROM(chID_min).c_str());
    wifi->SSID = read_EEPROM(ssid_min);
    wifi->PASSWORD = read_EEPROM(password_min);
    Serial.println("Dados lidos!");

    while (!connectWifi(wifi))
    {
      Serial.println("Impossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
      inconsistent_data = 1;
      setupBluetooth();
      writeBT(CONNECTION_ERROR);
      getWiFi(wifi);
    }
    Serial.println("IRIS foi conectada com sucesso!");
    writeBT(SUCCESSFULLY_CONNECTED);

    if (inconsistent_data)
    {
      Serial.println("Dados salvos na EEPROM");
      write_EEPROM(String(gtw->localAddr), loChID_min);
      write_EEPROM(String(gtw->destAddr), chID_min);
      write_EEPROM(wifi->SSID, ssid_min);
      write_EEPROM(wifi->PASSWORD, password_min);
      bleDisable();
    }
    Serial.println("Conectado na rede!");
    delay(1000);
  }
  else
  {
    Serial.println("Não há dados na EEPROM");
    setupBluetooth();
    getFirebase(gtw);
    getWiFi(wifi);

    while (!connectWifi(wifi))
    {
      Serial.println("Impossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
      writeBT(CONNECTION_ERROR); // Não foi possível conectar no WIFI
      getWiFi(wifi);
    }
    writeBT(SUCCESSFULLY_CONNECTED);

    Serial.println("IRIS foi conectada com sucesso!");
    write_EEPROM(String(gtw->localAddr), loChID_min);
    write_EEPROM(String(gtw->destAddr), chID_min);
    write_EEPROM(wifi->SSID, ssid_min);
    write_EEPROM(wifi->PASSWORD, password_min);
    Serial.println("Dados salvos na EEPROM!");

    delay(1000);
    bleDisable();
  }
}

void getWiFi(networkWiFi *wifi)
{
  writeBT(APP_SENDS_SSID);
  wifi->SSID = getData();
  writeBT(APP_SENDS_PASSWORD);
  wifi->PASSWORD = getData();
}

void getFirebase(networkLora *gtw)
{
  writeBT(APP_SENDS_USERID);
  //firebase->UserID = getData();
  gtw->localAddr = atol(writeBT(String(getChipID())).c_str());
}