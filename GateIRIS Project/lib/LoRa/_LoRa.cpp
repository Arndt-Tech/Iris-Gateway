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
  LoRa.setSpreadingFactor(12);
  gtw->stationCursor = 0;
  gtw->sendPacket.packetSize = 0;
}

void runningLoRa(networkLora *gtw, networkFirebase *fb)
{
  static unsigned long tLoRaSend = 0;
  if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
  {
    send_LoRa_Message(gtw, fb);
    tLoRaSend = xTaskGetTickCount();
  }
  gtw->sendPacket.packetSize = LoRa.parsePacket();
  receive_LoRa_Message(gtw, fb);
  gtw->stationCursor += 1;
  if (gtw->stationCursor >= fb->TOTAL_STATIONS)
    gtw->stationCursor = 0;
}

void send_LoRa_Message(networkLora *gtw, networkFirebase *fb)
{
  gtw->sendPacket.packetSize = sizeof(*fb->STATION_ID[gtw->stationCursor]) +
                               sizeof(gtw->sendPacket) - 2;
  gtw->sendPacket.isOn = fb->STATION_ID[gtw->stationCursor][ISON];
  Serial.println(gtw->sendPacket.isOn);
  LoRa.beginPacket();
  // Destino Addr
  LoRa.write(*fb->STATION_ID[gtw->stationCursor]);
  LoRa.write(*fb->STATION_ID[gtw->stationCursor] >> 8 & 0xFF);
  LoRa.write(*fb->STATION_ID[gtw->stationCursor] >> 16 & 0xFF);
  LoRa.write(*fb->STATION_ID[gtw->stationCursor] >> 24 & 0xFF);
  // Local Addr
  LoRa.write(gtw->sendPacket.localAddr);
  LoRa.write(gtw->sendPacket.localAddr >> 8 & 0xFF);
  LoRa.write(gtw->sendPacket.localAddr >> 16 & 0xFF);
  LoRa.write(gtw->sendPacket.localAddr >> 24 & 0xFF);
  // Estado
  LoRa.write(gtw->sendPacket.isOn);
  // Tamanho
  LoRa.write(gtw->sendPacket.packetSize);
  // Fim
  LoRa.endPacket();
  // Seto a flag pendência de retorno da estação
  fb->STATION_ID[gtw->stationCursor][RETURN] = 1;
}

String receive_LoRa_Message(networkLora *gtw, networkFirebase *fb)
{
  if (gtw->sendPacket.packetSize == 0)
    return "[ERR-LORA:NO PACKAGE]";
  for (register int i = 0; i < 4; i++)
    gtw->receivedPacket.dest_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    gtw->receivedPacket.sender_addr[i] = LoRa.read();
  for (int i = 0; i < fb->TOTAL_STATIONS; i++)
  {
    if (asm_addr(gtw->receivedPacket.sender_addr) == *fb->STATION_ID[i])
    {
      gtw->receivedPacket.fSender = 1;
      gtw->receivedPacket.iterator = i;
      break;
    }
  }
  if (asm_addr(gtw->receivedPacket.dest_addr) != gtw->sendPacket.localAddr || gtw->receivedPacket.fSender == 0)
    return "[ERR-LORA:IGNORED PACKAGE]"; // Pacote ignorado
  gtw->receivedPacket.aux_hmdt = LoRa.read();
  gtw->receivedPacket.aux_temp[0] = LoRa.read();
  gtw->receivedPacket.aux_temp[1] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    gtw->receivedPacket.latitude[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    gtw->receivedPacket.longitude[i] = LoRa.read();
  gtw->receivedPacket.packageLength = LoRa.read();
  if (gtw->receivedPacket.packageLength != gtw->sendPacket.packetSize)
    return "[ERR-LORA:INCONSISTENT PACKAGE]"; // Pacote inconsistente
  org_FB_data(gtw, fb);
  return "";
}

void org_FB_data(networkLora *gtw, networkFirebase *fb)
{
  uint16_t _aux_temp = 0;
  int8_t __packet_signal = LoRa.packetRssi(), __aux_packet_signal = 0;
  _aux_temp |= gtw->receivedPacket.aux_temp[0];
  _aux_temp |= gtw->receivedPacket.aux_temp[1] << 8;
  if (__packet_signal >= -35)
    __aux_packet_signal = 4;
  else if (__packet_signal < -35 && __packet_signal >= -50)
    __aux_packet_signal = 3;
  else if (__packet_signal < -50 && __packet_signal >= -80)
    __aux_packet_signal = 2;
  else if (__packet_signal < -80 && __packet_signal >= -121)
    __aux_packet_signal = 1;
  else if (__packet_signal < -122 || !fb->STATION_ID[gtw->receivedPacket.iterator][ISCONNECTED])
    __aux_packet_signal = 0;
  fb->STATION_ID[gtw->receivedPacket.iterator][RETURN] = 0;
  fb->STATION_ID[gtw->receivedPacket.iterator][STATION_SIGNAL] = __aux_packet_signal;
  fb->STATION_ID[gtw->receivedPacket.iterator][FB_HUMIDITY] = gtw->receivedPacket.aux_hmdt;
  fb->STATION_ID[gtw->receivedPacket.iterator][FB_TEMPERATURE] = _aux_temp;
  if (asm_addr(gtw->receivedPacket.latitude) != 0.000000)
    fb->STATION_ID[gtw->receivedPacket.iterator][FB_LATITUDE] = asm_addr(gtw->receivedPacket.latitude);
  if (asm_addr(gtw->receivedPacket.longitude) != 0.000000)
    fb->STATION_ID[gtw->receivedPacket.iterator][FB_LONGITUDE] = asm_addr(gtw->receivedPacket.longitude);
}

void verify_LoRa_Timeout(networkFirebase *fb)
{
  uint8_t rstTmt = 0;
  for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
  {
    if (fb->STATION_ID[i][RETURN])
    {
      rstTmt = 0;
      fb->STATION_ID[i][ISCONNECTED] = 0;
    }
    else if (!fb->STATION_ID[i][RETURN])
    {
      rstTmt = 1;
      fb->STATION_ID[i][ISCONNECTED] = 1;
    }
  }
  static unsigned long tPend = 0;
  if ((xTaskGetTickCount() - tPend) >= loraTmt && !rstTmt)
  {
    fb->TIMEOUT = 1;
    tPend = xTaskGetTickCount();
  }
  else if (rstTmt)
  {
    fb->TIMEOUT = 0;
    tPend = xTaskGetTickCount();
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
