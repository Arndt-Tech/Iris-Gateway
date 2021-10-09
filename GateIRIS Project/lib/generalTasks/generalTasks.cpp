// Inclusões
#include "generalTasks.h"

// Funções
void configBegin()
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
}

void setupDataSystem(networkLora *gtw, networkWiFi *wifi, networkFirebase *fb)
{
  bool inconsistent_data = 0;
  if ((verifyEEPROM(loChID_min) && verifyEEPROM(ssid_min) && verifyEEPROM(password_min) && verifyEEPROM(UserID_min)))
  {
    Serial.println("Há dados na EEPROM");
    gtw->localAddr = readEEPROM(loChID_min);
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
      writeEEPROM(gtw->localAddr, loChID_min);
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
    writeEEPROM(gtw->localAddr, loChID_min);
    writeStrEEPROM(wifi->SSID, ssid_min);
    writeStrEEPROM(wifi->PASSWORD, password_min);
    writeStrEEPROM(fb->USER_ID, UserID_min);
    Serial.println("Dados salvos na EEPROM!");
    vTaskDelay(1000);
    resetModule();
  }
  bleDisable();
}

void getWiFi(networkWiFi *wifi)
{
  writeBT(APP_SENDS_SSID);
  wifi->SSID = getData();
  writeBT(APP_SENDS_PASSWORD);
  wifi->PASSWORD = getData();
}

void getFirebase(networkLora *gtw, networkFirebase *fb)
{
  writeBT(APP_SENDS_USERID);
  fb->USER_ID = getData();
  gtw->localAddr = atol(writeBT(String(getChipID())).c_str());
  fb->GATEWAY_ID = gtw->localAddr;
}

void shift_vector(int cursor, int max, String *vector)
{
  for (int i = cursor; i < max; i++)
  {
    vector[i] = vector[i + 1];
    vector[i + 1] = "";
  }
}

void stationSeeker(networkFirebase *fb)
{
  if (!readStation(fb))
    Serial.println("ERROR");
}

void setStatus(networkFirebase *fb)
{
  FirebaseData FIREBASE_ISCONN;
  if (*fb->STATION_ID[TIMEOUT])
  {
    *fb->STATION_ID[TIMEOUT] = 0;
    for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
      if (!fb->STATION_ID[i][ISCONNECTED])
        Firebase.setBool(FIREBASE_ISCONN, CENTER_ISCONN_PREPROCESS, false);
  }
  else
    for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
      if (fb->STATION_ID[i][ISCONNECTED])
        Firebase.setBool(FIREBASE_ISCONN, CENTER_ISCONN_PREPROCESS, true);
}