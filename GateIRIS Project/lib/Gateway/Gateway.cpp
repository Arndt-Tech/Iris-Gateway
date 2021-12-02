#include "Gateway.hpp"

add::Additional gtw::Gateway::m_additional;
per::GPIO gtw::Gateway::m_gpio;
com::BLE gtw::Gateway::m_ble;
com::Wifi gtw::Gateway::m_wifi;
com::FirebaseServer gtw::Gateway::m_firebase;
com::Lora gtw::Gateway::m_lora;

add::Additional &gtw::Gateway::Additional() { return m_additional; }

per::GPIO &gtw::Gateway::manage::GPIO() { return m_gpio; }

com::BLE &gtw::Gateway::manage::Bluetooth() { return m_ble; }

com::Wifi &gtw::Gateway::manage::Wifi() { return m_wifi; }

com::FirebaseServer &gtw::Gateway::manage::Firebase() { return m_firebase; }

com::Lora &gtw::Gateway::manage::LoRa() { return m_lora; }


void gtw::Gateway::begin()
{
  pinMode(resetEEPROM, INPUT);
  cfg::Log::begin();
  checkSystem();
  m_firebase.begin();
  m_lora.begin();
}

void gtw::Gateway::checkSystem()
{
  if (readData())
    tryToEnter();
  else
    requestData();
}

bool gtw::Gateway::readData()
{
  if ((cfg::Log::check(loChID_min) && cfg::Log::check(ssid_min) && cfg::Log::check(password_min) && cfg::Log::check(UserID_min)))
  {
    com::Lora::package.send.set.localAddress(cfg::Log::read(loChID_min));
    com::Wifi::set::SSID(cfg::Log::reads(ssid_min));
    com::Wifi::set::password(cfg::Log::reads(password_min));
    com::FirebaseServer::set::userID(cfg::Log::reads(UserID_min));
    com::FirebaseServer::set::gatewayID(cfg::Log::read(loChID_min));
    return 1;
  }
  return 0;
}

void gtw::Gateway::tryToEnter()
{
  uint8_t inconsistent_data = 0;
  while (!m_wifi.connect())
  {
    inconsistent_data = 1;
    m_ble.begin();
  }
  m_ble.write(SUCCESSFULLY_CONNECTED);
  if (inconsistent_data)
  {
    cfg::Log::write(spc::SpecialFunctions::getChipID(), loChID_min);
    cfg::Log::write(m_wifi.get.SSID(), ssid_min);
    cfg::Log::write(m_wifi.get.password(), password_min);
    cfg::Log::write(m_firebase.get.userID(), UserID_min);
    spc::SpecialFunctions::resetModule();
  }
  vTaskDelay(500);
}

void gtw::Gateway::requestData()
{
  m_ble.begin();
  while (!m_wifi.connect())
  {
    m_ble.write(CONNECTION_ERROR); // Não foi possível conectar no WIFI
    m_ble.wifi();
  }
  m_ble.write(SUCCESSFULLY_CONNECTED);

  cfg::Log::write(spc::SpecialFunctions::getChipID(), loChID_min);
  cfg::Log::write(m_wifi.get.SSID(), ssid_min);
  cfg::Log::write(m_wifi.get.password(), password_min);
  cfg::Log::write(m_firebase.get.userID(), UserID_min);

  vTaskDelay(1000);
  spc::SpecialFunctions::resetModule();
}

/*
void gtw::Gateway::checkSystem()
{
  uint8_t inconsistent_data = 0;
  if ((cfg::Log::check(loChID_min) && cfg::Log::check(ssid_min) && cfg::Log::check(password_min) && cfg::Log::check(UserID_min)))
  {
#if _DEBUG_MODE_
    Serial.println("Há dados na EEPROM");
#elif !_DEBUG_MODE_
#endif
    com::Lora::package.send.set.localAddress(cfg::Log::read(loChID_min));
    com::Wifi::set::SSID(cfg::Log::reads(ssid_min));
    com::Wifi::set::password(cfg::Log::reads(password_min));
    com::FirebaseServer::set::userID(cfg::Log::reads(UserID_min));
    com::FirebaseServer::set::gatewayID(cfg::Log::read(loChID_min));
#if _DEBUG_MODE_
    Serial.println("Dados lidos!");
#elif !_DEBUG_MODE_
#endif
    while (!m_wifi.connect())
    {
#if _DEBUG_MODE_
      Serial.println("\nImpossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
#elif !_DEBUG_MODE_
#endif
      inconsistent_data = 1;
      m_ble.begin();
    }
#if _DEBUG_MODE_
    Serial.println("IRIS foi conectada com sucesso!");
#elif !_DEBUG_MODE_
#endif
    m_ble.write(SUCCESSFULLY_CONNECTED);
    if (inconsistent_data)
    {
#if _DEBUG_MODE_
      Serial.println("Dados salvos na EEPROM");
#elif !_DEBUG_MODE_
#endif
      cfg::Log::write(spc::SpecialFunctions::getChipID(), loChID_min);
      cfg::Log::write(m_wifi.get.SSID(), ssid_min);
      cfg::Log::write(m_wifi.get.password(), password_min);
      cfg::Log::write(m_firebase.get.userID(), UserID_min);
      spc::SpecialFunctions::resetModule();
    }
#if _DEBUG_MODE_
    Serial.println("Conectado na rede!");
#elif !_DEBUG_MODE_
#endif
    vTaskDelay(500);
  }
  else
  {
#if _DEBUG_MODE_
    Serial.println("Não há dados na EEPROM");
#elif !_DEBUG_MODE_
#endif
    m_ble.begin();
    while (!m_wifi.connect())
    {
#if _DEBUG_MODE_
      Serial.println("\nImpossível conectar na rede");
      Serial.println("Reescreva o SSID e a SENHA");
#elif !_DEBUG_MODE_
#endif
      m_ble.write(CONNECTION_ERROR); // Não foi possível conectar no WIFI
      m_ble.wifi();
    }
    m_ble.write(SUCCESSFULLY_CONNECTED);
#if _DEBUG_MODE_
    Serial.println("IRIS foi conectada com sucesso!");
#elif !_DEBUG_MODE_
#endif
    cfg::Log::write(spc::SpecialFunctions::getChipID(), loChID_min);
    cfg::Log::write(m_wifi.get.SSID(), ssid_min);
    cfg::Log::write(m_wifi.get.password(), password_min);
    cfg::Log::write(m_firebase.get.userID(), UserID_min);
#if _DEBUG_MODE_
    Serial.println("Dados salvos na EEPROM!");
#elif !_DEBUG_MODE_
#endif
    vTaskDelay(1000);
    spc::SpecialFunctions::resetModule();
  }
}
*/