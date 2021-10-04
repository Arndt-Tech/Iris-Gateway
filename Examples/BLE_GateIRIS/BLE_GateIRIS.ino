//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
// Inclusões de bibliotecas
  #include <BLEDevice.h>
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>
  #include "esp_gatts_api.h"

// ID's de serviço e características BT
  #define SERVICE_UUID           "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
  #define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8"
  #define CHARACTERISTIC_UUID_RX "808ccec4-d862-11eb-b8bc-0242ac130003"

// Senhas para protocolo de segurança
  #define requestClientAppBT  "tSdcxhoLVYj7oMT500&5"      // Recebe do app
  #define passwordClientAppBT "KQ.?2qf01X|,JZRq8!},"      // Envia para o app



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|                             
// FUNÇÕES
  // GET DATA BT
  String getData();            // Recebe dados com callback
  String readBT();             // Recebe dados sem callback

  // ENVIO DE DADOS BT
  void writeBT(String dados);  // envia dados via bluetooth

  // CONEXÃO BT
  void waitingBT();            // Aguarda bluetooth conectar
  void waitingSYNC();          // Aguarda sincronização da comunicação
  void refreshConnectionBT();  // Atualiza estado de conexão bluetooth com callback

  // REQUEST BT
  void waitingREQUEST();       // Aguarda requisição do clientAPP
  bool getRequestBT();         // Recebe requisição com callback
  void sendREQUEST();          // Envia requisição para clientAPP

  // BT WIFI_DATA
  void waitingSSID_WIFI();     // Espera clientAPP enviar SSID
  void waitingPASSWORD_WIFI(); // Espera clientAPP enviar PASSWORD


//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
// VARIÁVEIS  
  // String's
  String packBT= ""; // Buffer virtual, usado no callback
  String SSID_ = ""; // String onde fica alocado o SSID_WIFI 
  String PASS_ = ""; // String onde fica alocado a PASSWORD_WIFI

  // Boolean's
  bool connectedBT = 0;         // Estado de conexão bluetooth, usado no callback
  bool oldDeviceConnected = 0;  // Estado antigo de conexão bluetooth, usado no refreshConnectionBT();
  
  // Outros dados
  BLEServer* serverBT = NULL;                   // Aloca server BT
  BLECharacteristic* characteristic_TX = NULL;  // Aloca característica BT_TX
  BLECharacteristic* characteristic_RX = NULL;  // Aloca característica BT_RX
  uint32_t BT_rxData;                           // Aloca função de recebimento no callback



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
// CALLBACK'S
  // Estado de conexão bluetooth
  class CallbackServer: public BLEServerCallbacks 
  {
    void onConnect(BLEServer* serverBT) {connectedBT = 1;}  // Dispositivo conectado
    void onDisconnect(BLEServer* serverBT) {connectedBT = 0;}   // Dispositivo desconectado
  };
  
  // Recebimento de dados bluetooth
  class CallbackRX: public BLECharacteristicCallbacks 
  {
    void onWrite(BLECharacteristic *characteristic_TX) 
    {
      packBT = "";
      std::string BT_rxData = characteristic_RX->getValue();
      if (BT_rxData.length() > 0) 
      {
        for (int i = 0; i < BT_rxData.length(); i++)packBT += (char)(BT_rxData[i]);
        Serial.println ("Buffer: " + String (packBT) + "\nTamanho: " + String(packBT.length()) + "\n");
      }
    }
  };



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setup() 
{
  delay (1000);
  Serial.begin (115200);
  Serial.println ("Iniciando...");
  delay (2000);
  
  setupBluetooth();       // Inicializa bluetooth low energy
  
  waitingBT();            // Espera conexão com clientAPP

  waitingSYNC();          // Espera sincronização do clientAPP
  
  sendREQUEST();          // Envia request para o clientAPP
  
  waitingREQUEST();       // Espera para receber o request do clientAPP

  waitingSSID_WIFI();     // Espera clientAPP enviar SSID da rede
  
  waitingPASSWORD_WIFI(); // Espera clientAPP enviar PASSWORD da rede
}

void loop() 
{
  refreshConnectionBT();
  Serial.println (SSID_);
  delay (1000);
  Serial.println (PASS_);
  delay (1000);
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupBluetooth()
{
  BLEDevice::init("GateIRIS");
  
  // Cria server
  serverBT = BLEDevice::createServer();
  
  // Seta callback, para idenficar se o dispositivo está conectado
  serverBT->setCallbacks(new CallbackServer());
  
  // Cria um serviço
  BLEService* serviceBT = serverBT->createService(SERVICE_UUID);

  // Cria uma caracteristica de serviço para TX
  characteristic_TX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ);   // Cria caracteristica TX

  // Cria uma caracteristica de serviço para RX
  characteristic_RX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  
  // Seta callback de recebimento
  characteristic_RX->setCallbacks(new CallbackRX());

  // Adiciona um descritor RX
  characteristic_RX->addDescriptor(new BLE2902());

  // Inicia serviço
  serviceBT->start();

  // Cria a propagação do dispositivo no serviço "SERVICE_UUID" 
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  
  // Inicia propagação do dispositivo
  BLEDevice::startAdvertising();
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String readBT()             // Função de leitura solitária
{
  String _data = "";
  if (connectedBT)
  {
    std::string dataBT = characteristic_RX->getValue();
    if (dataBT.length() > 0) for (int i = 0; i < dataBT.length(); i++)_data += (char)(dataBT[i]); return (_data);
  }
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void writeBT(String dados)
{
  unsigned long timeBT;
  if (connectedBT)
  {
    if ((millis() - timeBT) > 4)
    {
      characteristic_TX->setValue(dados.c_str());
      characteristic_TX->notify();
      timeBT = millis();
    }
  }
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String getData()        // Função de leitura conjunta com callback
{ 
  packBT = "";     
  while (1)
  { 
    refreshConnectionBT(); 
    if (connectedBT) 
    {
      if (packBT.length() != 0 && (packBT.length() == (characteristic_RX->getValue()).length())) return (packBT);
    }
  }
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
bool getRequestBT()
{
  while (1)
  {  
    refreshConnectionBT();
    if (connectedBT) 
    {
      if (packBT.length() != 0 && (packBT.length() == (characteristic_RX->getValue()).length())) 
      {
        if (packBT == requestClientAppBT) return 1;
        else if (packBT != requestClientAppBT) 
        {
          packBT = "";
          return 0;
        }
      }
    }
  }
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void refreshConnectionBT()
{
  // Atualiza conexão -> desconectado
  if (!connectedBT && oldDeviceConnected) 
  {
    Serial.println ("Bluetooth desconectado!");
    delay(500);
    packBT = "";    // Limpa o buffer
    serverBT->startAdvertising(); 
    oldDeviceConnected = connectedBT;
  }
    
  // Atualiza conexão -> conectado
  if (connectedBT && !oldDeviceConnected) 
  {
    Serial.println ("Bluetooth conectado!");
    oldDeviceConnected = connectedBT;
  }
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingBT()   // Aguarda bluetooth conectar
{
  Serial.println ("Esperando conexão bluetooth...");
  while (!connectedBT)
  {
    Serial.print (".");
    delay (500);
  }
  Serial.println("Bluetooth conectado!"); 
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingSYNC()  // Aguarda sincronização da comunicação
{
  String sync = "";
  Serial.println ("Aguardando sincronização...");
  while (sync != "@")
  {
    refreshConnectionBT();
    sync = getData();
  }
  Serial.println ("Bluetooth sincronizado!");
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingREQUEST()  // Aguarda requisição do clientAPP
{
  do{
     refreshConnectionBT();
     if (connectedBT)Serial.println ("Esperando requisição...");
     delay (250);
  }while (!getRequestBT());
  Serial.println ("Requisição aceita: " + String(getRequestBT())); 
  delay (1000);
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void sendREQUEST()  // Envia requisição para clientAPP
{
  Serial.println ("Enviando request...");
  packBT = "";    // Limpeza do buffer
  writeBT (passwordClientAppBT);
  Serial.println ("Request enviado!");
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingSSID_WIFI()   // Espera clientAPP enviar SSID
{
  Serial.println ("SSID");
  SSID_ = getData();
  Serial.println ("SSID: " + String (SSID_));
  delay (1000);
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingPASSWORD_WIFI()  // Espera clientAPP enviar PASSWORD
{
  Serial.println ("SENHA");
  PASS_ = getData();
  Serial.println ("PASS: " + String (PASS_));
  delay (1000);
}
