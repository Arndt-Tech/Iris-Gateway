// Inclusões
#include "_LoRa.h"

void setupLoRa(networkLora *gtw)
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    Serial.println("Erro ao inicializar LoRa!");
    while (1)
      ;
  }
  LoRa.enableCrc();
  gtw->stationCursor = 0;
  gtw->packSize = 0;
}

void runningLoRa(networkLora *gtw, networkFirebase *fb)
{
  verify_LoRa_Timeout(fb);
  static unsigned long tLoRaSend = 0;
  if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
  {
    send_LoRa_Message(gtw, fb);
    tLoRaSend = xTaskGetTickCount();
  }
  gtw->packSize = LoRa.parsePacket();
  receive_LoRa_Message(gtw, fb);
  gtw->stationCursor += 1;
  if (gtw->stationCursor >= fb->TOTAL_STATIONS)
    gtw->stationCursor = 0;
  vTaskDelay(1);
}

void send_LoRa_Message(networkLora *gtw, networkFirebase *fb)
{
  uint8_t packLen = sizeof(atol((*fb->STATION_ID[gtw->stationCursor]).c_str())) +
                    sizeof(gtw->localAddr) +
                    sizeof(gtw->isOn) +
                    sizeof(packLen);
  if (fb->STATION_ID[gtw->stationCursor][ISON] == "false")
    gtw->isOn = 0;
  else if (fb->STATION_ID[gtw->stationCursor][ISON] == "true")
    gtw->isOn = 1;
  LoRa.beginPacket();
  // Destino Addr
  LoRa.write(atol((*fb->STATION_ID[gtw->stationCursor]).c_str()));
  LoRa.write(atol((*fb->STATION_ID[gtw->stationCursor]).c_str()) >> 8 & 0xFF);
  LoRa.write(atol((*fb->STATION_ID[gtw->stationCursor]).c_str()) >> 16 & 0xFF);
  LoRa.write(atol((*fb->STATION_ID[gtw->stationCursor]).c_str()) >> 24 & 0xFF);
  // Local Addr
  LoRa.write(gtw->localAddr);
  LoRa.write(gtw->localAddr >> 8 & 0xFF);
  LoRa.write(gtw->localAddr >> 16 & 0xFF);
  LoRa.write(gtw->localAddr >> 24 & 0xFF);
  // Estado
  LoRa.write(gtw->isOn);
  // Tamanho
  LoRa.write(packLen);
  // Fim
  LoRa.endPacket();
  gtw->packSize = packLen;
  // Seto a flag pendência de retorno da estação
  fb->STATION_ID[gtw->stationCursor][RETURN] = "1";
}

String receive_LoRa_Message(networkLora *gtw, networkFirebase *fb)
{
  if (gtw->packSize == 0)
    return "[ERR-LORA:NO PACKAGE]";
  _loraData aux_data;
  for (register int i = 0; i < 4; i++)
    aux_data.dest_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    aux_data.sender_addr[i] = LoRa.read();
  for (int i = 0; i < fb->TOTAL_STATIONS; i++)
  {
    if (String(asm_addr(aux_data.sender_addr)) == *fb->STATION_ID[i])
    {
      aux_data.fSender = 1;
      aux_data.iterator = i;
      break;
    }
  }
  if (asm_addr(aux_data.dest_addr) != gtw->localAddr || aux_data.fSender == 0)
    return "[ERR-LORA:IGNORED PACKAGE]"; // Pacote ignorado
  aux_data.aux_hmdt = LoRa.read();
  aux_data.aux_temp[0] = LoRa.read();
  aux_data.aux_temp[1] = LoRa.read();
  aux_data.packageLength = LoRa.read();
  if (aux_data.packageLength != gtw->packSize)
    return "[ERR-LORA:INCONSISTENT PACKAGE]"; // Pacote inconsistente
  org_FB_data(&aux_data, fb);

  Serial.println("Destino: " + String(asm_addr(aux_data.dest_addr)));
  Serial.println("Remetente: " + String(asm_addr(aux_data.sender_addr)));
  Serial.println("Umidade: " + String(fb->STATION_ID[aux_data.iterator][FB_HUMIDITY]));
  Serial.println("Temperatura: " + String(fb->STATION_ID[aux_data.iterator][FB_TEMPERATURE]));
  Serial.println("Tamanho informado: " + String(aux_data.packageLength));
  Serial.println("Tamanho identificado: " + String(gtw->packSize));
  Serial.println("");
  return "";
}

void org_FB_data(_loraData *__data, networkFirebase *fb)
{
  uint16_t _aux_temp = 0;
  _aux_temp |= __data->aux_temp[0] & 0xFF;
  _aux_temp |= __data->aux_temp[1] << 8 & 0xFF;

  fb->STATION_ID[__data->iterator][RETURN] = "0";
  fb->STATION_ID[__data->iterator][FB_HUMIDITY] = String(__data->aux_hmdt);
  fb->STATION_ID[__data->iterator][FB_TEMPERATURE] = String((float)_aux_temp / 10, 1);
}

void verify_LoRa_Timeout(networkFirebase *fb)
{
  static unsigned long tPend = 0;
  if ((millis() - tPend) >= loraTmt)
  {
    for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
      if (fb->STATION_ID[i][RETURN] == "1")
        fb->STATION_ID[i][ISCONNECTED] = "false";
      else if (fb->STATION_ID[i][RETURN] == "0")
        fb->STATION_ID[i][ISCONNECTED] = "true";
    tPend = millis();
  }
}

uint32_t asm_addr(uint8_t *addr)
{
  uint32_t newAddr = 0;
  newAddr |= addr[0];
  newAddr |= addr[1] << 8;
  newAddr |= addr[2] << 16;
  newAddr |= addr[3] << 24;
  return newAddr;
}
/*
  Serial.println("Recebido de: 0x" + String(sender, HEX));
  Serial.println("Enviado para: 0x" + String(requisitor, HEX));
  Serial.println("Tamanho do pacote: " + String(incomingLength));
  Serial.println("Pacote recebido: " + incoming);
  Serial.println("dBm: " + String(LoRa.packetRssi()));
  Serial.println("Ruido: " + String(LoRa.packetSnr()));
  Serial.println();
  */