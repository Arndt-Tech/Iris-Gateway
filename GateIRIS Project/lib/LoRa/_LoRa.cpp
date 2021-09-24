// Inclusões
#include "_LoRa.h"

// Variáveis externas
int erros, pacotes, enviados;

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
  gtw->packSize = 0;
  gtw->stationCursor = 0;
  gtw->incomingPack = "";
  LoRa.enableCrc();
}

void runningLoRa(networkLora *gtw, networkFirebase *fb)
{
  static unsigned long tLoRaSend = 0;
  if ((millis() - tLoRaSend) > INTERVAL)
  {
    send_LoRa_Message(gtw, fb);
    tLoRaSend = millis();
  }
  gtw->packSize = LoRa.parsePacket();
  receive_LoRa_Message(gtw);
  gtw->stationCursor += 1;
  if (gtw->stationCursor >= fb->TOTAL_STATIONS)
    gtw->stationCursor = 0;
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
  enviados++;
  // Seto a flag timeout de requisição
  for (int i = 0; i < fb->TOTAL_STATIONS; i++)
  {
    if (String(gtw->localAddr) == *fb->STATION_ID[i])
      fb->STATION_ID[i][RETURN] = 1;
  }
}

String receive_LoRa_Message(networkLora *gtw)
{
  if (gtw->packSize == 0)
    return "";
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  gtw->incomingPack = "";
  pacotes++;
  for (register int i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  incomingLength += LoRa.read();

  while (LoRa.available())
    gtw->incomingPack += (char)LoRa.read();
  if (incomingLength != gtw->incomingPack.length())
  {
    erros++;
    return ""; // Pacote incosistente
  }
  if (asm_addr(to_who_addr) != gtw->localAddr || asm_addr(sender_addr) != gtw->destAddr)
    return ""; // Pacote ignorado
  return gtw->incomingPack;
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