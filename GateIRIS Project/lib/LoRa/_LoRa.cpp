//
#include "_Lora.h"

uint32_t aux::LoraPackage::snd::m_dest_addr;
uint32_t aux::LoraPackage::snd::m_local_addr;
uint8_t aux::LoraPackage::snd::m_is_on;
uint8_t aux::LoraPackage::snd::m_size;
uint32_t aux::LoraPackage::rcv::m_receiver_addr;
uint32_t aux::LoraPackage::rcv::m_sender_addr;
int32_t aux::LoraPackage::rcv::m_latitude;
int32_t aux::LoraPackage::rcv::m_longitude;
uint8_t aux::LoraPackage::rcv::m_humidity;
uint8_t aux::LoraPackage::rcv::m_temperature;
uint8_t aux::LoraPackage::rcv::m_size;
uint8_t aux::LoraPackage::rcv::m_iterator;
int8_t aux::LoraPackage::rcv::m_signal;

aux::LoraPackage com::Lora::package;
uint8_t com::Lora::m_selectedStation = 0;

void com::Lora::begin()
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
}

void com::Lora::opr::duplex()
{
  static unsigned long tLoRaSend = 0;
  if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
  {
    opr::sendPackage();
    tLoRaSend = xTaskGetTickCount();
  }
  opr::readPackage();
  m_selectedStation += 1;
  if (m_selectedStation >= com::FirebaseServer::get::totalStations())
    m_selectedStation = 0;
}

void com::Lora::opr::sendPackage()
{
  package.snd.m_size = (uint8_t)(sizeof(package.snd) - SIZE_CORRECTION(2));
  LoRa.beginPacket();
  // Destino Addr
  LoRa.write(com::FirebaseServer::get::station(m_selectedStation));
  LoRa.write(com::FirebaseServer::get::station(m_selectedStation) >> 8 & 0xFF);
  LoRa.write(com::FirebaseServer::get::station(m_selectedStation) >> 16 & 0xFF);
  LoRa.write(com::FirebaseServer::get::station(m_selectedStation) >> 24 & 0xFF);
  // Local Addr
  LoRa.write(package.snd.m_local_addr);
  LoRa.write(package.snd.m_local_addr >> 8 & 0xFF);
  LoRa.write(package.snd.m_local_addr >> 16 & 0xFF);
  LoRa.write(package.snd.m_local_addr >> 24 & 0xFF);
  // Estado
  LoRa.write(package.snd.m_is_on);
  // Tamanho
  LoRa.write(package.snd.m_size);
  // Fim
  LoRa.endPacket();
  // Seto a flag pendência de retorno da estação
  com::FirebaseServer::set::station(m_selectedStation, RETURN, 1);
}

void com::Lora::opr::readPackage()
{
  uint8_t __identified_size = LoRa.parsePacket();
  if (!__identified_size)
    return;
  uint8_t __receive_addr[4] = {0}, __sender_addr[4] = {0};
  uint8_t __latitude[4] = {0}, __longitude[4] = {0};
  uint8_t __sender = 0, __iterator = 0, __size_received = 0;
  int8_t __humidity = 0, __temperature[2] = {0};
  for (register uint8_t i = 0; i < 4; i++)
    __receive_addr[i] = LoRa.read();
  for (register uint8_t i = 0; i < 4; i++)
    __sender_addr[i] = LoRa.read();
  for (register uint8_t i = 0; i < com::FirebaseServer::get::totalStations(); i++)
  {
    if (spc::SpecialFunctions::asmAddr(__sender_addr) == com::FirebaseServer::get::station(i))
    {
      __sender = 1;
      __iterator = i;
      break;
    }
  }
  if (spc::SpecialFunctions::asmAddr(__receive_addr) != package.snd.m_local_addr || __sender == 0)
    return;
  __humidity = LoRa.read();
  __temperature[0] = LoRa.read();
  __temperature[1] = LoRa.read();
  for (register uint8_t i = 0; i < 4; i++)
    __latitude[i] = LoRa.read();
  for (register uint8_t i = 0; i < 4; i++)
    __longitude[i] = LoRa.read();
  __size_received = LoRa.read();
  if (__size_received != __identified_size)
    return;

  package.rcv.m_sender_addr = spc::SpecialFunctions::asmAddr(__sender_addr);
  package.rcv.m_humidity = __humidity;
  package.rcv.m_temperature |= __temperature[0];
  package.rcv.m_temperature |= __temperature[1] << 8;
  package.rcv.m_latitude = spc::SpecialFunctions::asmAddr(__latitude);
  package.rcv.m_longitude = spc::SpecialFunctions::asmAddr(__longitude);
  package.rcv.m_size = __size_received;
  package.rcv.m_signal = LoRa.packetRssi();
  package.rcv.m_iterator = __iterator;

  return;
}

inline void com::Lora::organizeData()
{
  int8_t __aux_packet_signal = 0;
  if (package.rcv.m_signal >= -35)
    __aux_packet_signal = 4;
  else if (package.rcv.m_signal < -35 && package.rcv.m_signal >= -50)
    __aux_packet_signal = 3;
  else if (package.rcv.m_signal < -50 && package.rcv.m_signal >= -80)
    __aux_packet_signal = 2;
  else if (package.rcv.m_signal < -80 && package.rcv.m_signal >= -121)
    __aux_packet_signal = 1;
  else if (package.rcv.m_signal < -122 || !com::FirebaseServer::get::station(package.rcv.m_iterator, ISCONNECTED))
    __aux_packet_signal = 0;
  com::FirebaseServer::set::station(package.rcv.m_iterator, RETURN, 0);
  com::FirebaseServer::set::station(package.rcv.m_iterator, STATION_SIGNAL, __aux_packet_signal);
  com::FirebaseServer::set::station(package.rcv.m_iterator, FB_HUMIDITY, package.rcv.m_humidity);
  com::FirebaseServer::set::station(package.rcv.m_iterator, FB_TEMPERATURE, package.rcv.m_temperature);
  if (package.rcv.m_latitude != 0.000000)
    com::FirebaseServer::set::station(package.rcv.m_iterator, FB_LATITUDE, package.rcv.m_latitude);
  if (package.rcv.m_longitude != 0.000000)
    com::FirebaseServer::set::station(package.rcv.m_iterator, FB_LONGITUDE, package.rcv.m_longitude);
}

void com::Lora::checkTimeout()
{
  uint8_t rstTmt = 0;
  for (uint8_t i = 0; i < com::FirebaseServer::get::totalStations(); i++)
  {
    if (com::FirebaseServer::get::station(i, RETURN))
    {
      rstTmt = 0;
      com::FirebaseServer::set::station(i, ISCONNECTED, 0);
    }
    else if (!com::FirebaseServer::get::station(i, RETURN))
    {
      rstTmt = 1;
      com::FirebaseServer::set::station(i, ISCONNECTED, 1);
    }
  }
  static unsigned long tPend = 0;
  if ((spc::SpecialFunctions::ctrlTickCount(xTaskGetTickCount(), tPend)) >= loraTmt && !rstTmt)
  {
    com::FirebaseServer::set::timeout(1);
    tPend = xTaskGetTickCount();
  }
  else if (rstTmt)
  {
    com::FirebaseServer::set::timeout(0);
    tPend = xTaskGetTickCount();
  }
}

uint32_t aux::LoraPackage::send::get::destinationAddress() { return snd::m_dest_addr; }

uint32_t aux::LoraPackage::send::get::localAddress() { return snd::m_local_addr; }

uint8_t aux::LoraPackage::send::get::valveStatus() { return snd::m_is_on; }

uint8_t aux::LoraPackage::send::get::size() { return snd::m_size; }

void aux::LoraPackage::send::set::destinationAddress(uint32_t value) { snd::m_dest_addr = value; }

void aux::LoraPackage::send::set::localAddress(uint32_t value) { snd::m_local_addr = value; }

void aux::LoraPackage::send::set::valveStatus(uint8_t status) { snd::m_is_on = status; }

uint32_t aux::LoraPackage::receive::get::receiveAddress() { return rcv::m_receiver_addr; }

uint32_t aux::LoraPackage::receive::get::senderAddress() { return rcv::m_sender_addr; }

int32_t aux::LoraPackage::receive::get::latitude() { return rcv::m_latitude; }

int32_t aux::LoraPackage::receive::get::longitude() { return rcv::m_longitude; }

uint8_t aux::LoraPackage::receive::get::temperature() { return rcv::m_temperature; }

uint8_t aux::LoraPackage::receive::get::humidity() { return rcv::m_humidity; }

uint8_t aux::LoraPackage::receive::get::stationNumber() { return rcv::m_iterator; }

int8_t aux::LoraPackage::receive::get::signal() { return rcv::m_signal; }

uint8_t aux::LoraPackage::receive::get::size() { return rcv::m_size; }
