//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|  
// Inclusões  
  // Include de funções
  #include "_configBegin.h"
  #include "_OLED.h"
  #include "_Bluetooth.h"
  #include "_EEPROM.h"
  #include "_WiFi.h"
  #include "_LoRa.h"
  #include "_Firebase.h"
  #include "_multiCore.h"
  #include "_specialFunctions.h"
  #include "chipID.h"
  
//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|  
void setup() 
{ 
  // Inicia Dispositivo
  configBegin();
  //clearEEPROM(0, EEPROM_SIZE);

  // Inicia SSD1306
  setupOLED();
  
  // Inicia BLE
  setupBluetooth();  
  
  // Inicia WIFI
  setupWifi();

  // Inicia Firebase
  setupFirebase();

  // Inicia LoRa
  setupLoRa();
  
  // Setup taskLora core 0
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, 0);
}


//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|  
void loop() 
{
  reconnectWiFi(SSID_STR, PASSWORD_STR);
  Serial.println ("Heap Loop: " +  String(xPortGetFreeHeapSize()));
  if ((millis() - tFirebase) > 2000)
  {
    runningFirebase();
    interfacePacotesLora();
    tFirebase = millis();
  }
}




String getLoraUmidity(String string)
{
  string.remove(0, 7);
  return (string);
}



String getLoraTemp(String string)
{
  string.remove(0, 1);
  string.remove(5, 3);
  return (string);
}
