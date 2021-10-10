// Inclusões
#include "systemFunctions.h"

// Funções
void configBegin(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb, networkLora *gtw)
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  setupOLED();
  setupDataSystem(ble, wifi, fb, gtw);
  setupFirebase(fb);
  setupLoRa(gtw);
  setupTasks();
}

void setupDataSystem(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb, networkLora *gtw)
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
      setupBluetooth(ble);
      getFirebase(ble, fb, gtw);
      getWiFi(ble, wifi);
    }
    Serial.println("IRIS foi conectada com sucesso!");
    writeBT(ble, SUCCESSFULLY_CONNECTED);
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
    setupBluetooth(ble);
    getFirebase(ble, fb, gtw);
    getWiFi(ble, wifi);
    while (!connectWifi(wifi))
    {
      Serial.println("\nImpossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
      writeBT(ble, CONNECTION_ERROR); // Não foi possível conectar no WIFI
      getWiFi(ble, wifi);
    }
    writeBT(ble, SUCCESSFULLY_CONNECTED);
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

