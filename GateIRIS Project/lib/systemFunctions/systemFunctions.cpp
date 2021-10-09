// Inclusões
#include "systemFunctions.h"

// Funções
void configBegin(networkLora *gtw, networkWiFi *wifi, networkFirebase *fb)
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  setupOLED();
  setupDataSystem(gtw, wifi, fb);
  setupFirebase(fb);
  setupLoRa(gtw);
  setupTasks();
}

void setupDataSystem(networkLora *gtw, networkWiFi *wifi, networkFirebase *fb)
{
  bool inconsistent_data = 0;
  if ((verifyEEPROM(loChID_min) && verifyEEPROM(ssid_min) && verifyEEPROM(password_min) && verifyEEPROM(UserID_min)))
  {
    Serial.println("Há dados na EEPROM");
    gtw->sendPacket.localAddr = readEEPROM(loChID_min);
    wifi->SSID = readStrEEPROM(ssid_min);
    wifi->PASSWORD = readStrEEPROM(password_min);
    fb->USER_ID = readStrEEPROM(UserID_min);
    fb->GATEWAY_ID = readEEPROM(loChID_min);
    Serial.println("Dados lidos!");
    while (!connectWifi(wifi))
    {
      Serial.println("\nImpossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
      inconsistent_data = 1;
      setupBluetooth();
      getFirebase(gtw, fb);
      getWiFi(wifi);
    }
    Serial.println("IRIS foi conectada com sucesso!");
    writeBT(SUCCESSFULLY_CONNECTED);
    if (inconsistent_data)
    {
      Serial.println("Dados salvos na EEPROM");
      writeEEPROM(gtw->sendPacket.localAddr, loChID_min);
      writeStrEEPROM(wifi->SSID, ssid_min);
      writeStrEEPROM(wifi->PASSWORD, password_min);
      writeStrEEPROM(fb->USER_ID, UserID_min);
      resetModule();
    }
    Serial.println("Conectado na rede!");
    vTaskDelay(1000);
  }
  else
  {
    Serial.println("Não há dados na EEPROM");
    setupBluetooth();
    getFirebase(gtw, fb);
    getWiFi(wifi);
    while (!connectWifi(wifi))
    {
      Serial.println("\nImpossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
      writeBT(CONNECTION_ERROR); // Não foi possível conectar no WIFI
      getWiFi(wifi);
    }
    writeBT(SUCCESSFULLY_CONNECTED);
    Serial.println("IRIS foi conectada com sucesso!");
    writeEEPROM(gtw->sendPacket.localAddr, loChID_min);
    writeStrEEPROM(wifi->SSID, ssid_min);
    writeStrEEPROM(wifi->PASSWORD, password_min);
    writeStrEEPROM(fb->USER_ID, UserID_min);
    Serial.println("Dados salvos na EEPROM!");
    vTaskDelay(1000);
    resetModule();
  }
  bleDisable();
}

