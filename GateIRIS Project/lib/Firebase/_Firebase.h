#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <Firebase_ESP_Client.h>
#include "pinout.h"
#include "debug.h"
#include "specialFunctions.h"

#define FIREBASE_HOST "iris-24c17-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZQlCp7oKTm7QYupuhh9b2C8pqajd3CoxGwK9iAbl"
#define FIREBASE_API "AIzaSyDO-jkFKJze7Fz_z6cxps5kBQ4fMiwJa6s"

#define CENTER_REFRESH_STATIONS_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/refresh/"
#define CENTER_ID_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/"
#define CENTER_ISON_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/isOn/"
#define CENTER_ISCONN_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/isConnected/"
#define CENTER_RSSI_RTDB_CONN "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[stationCursor]) + "/rssi/"
#define CENTER_RSSI_RTDB_DISCONN "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/rssi/"
#define CENTER_TEMPERATURE_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/Data/" + "/temperature/"
#define CENTER_HUMIDITY_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/Data/" + "/humidity/"
#define CENTER_LATITUDE_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/Data/" + "/latitude/"
#define CENTER_LONGITUDE_RTDB "/Users/" + String(m_user_id) + "/Gateway/" + String(m_gateway_id) + "/Station/" + String(*m_station[i]) + "/Data/" + "/longitude/"

#define INIT_MAX_STATIONS 10
#define MAX_STATIONS ((INIT_MAX_STATIONS) / 2)
#define INIT_PARAMETERS 10

#define RETURN 1
#define ISON 2
#define ISCONNECTED 3
#define STATION_SIGNAL 4
#define FB_HUMIDITY 5
#define FB_TEMPERATURE 6
#define FB_LATITUDE 7
#define FB_LONGITUDE 8

#define TIME(a) (a)

namespace com
{
  class FirebaseServer
  {
  private:
    FirebaseConfig m_config;
    FirebaseAuth m_auth;
    static FirebaseData m_firebase_ids;
    static FirebaseData m_firebase_data;
    static String m_user_id;
    static uint32_t m_gateway_id;
    static int32_t m_station[INIT_MAX_STATIONS][INIT_PARAMETERS];
    static uint8_t m_total_stations;
    static uint8_t m_refresh_stations;
    static uint8_t m_sync_refresh;
    static uint8_t m_status;
    static uint8_t m_timeout;
    static String m_token;
    static token_info_t m_token_info;

  private:
    static void runConnectionStatusSystem();
    static uint8_t refreshStations();

  public:
    void begin();
    struct opr
    {
      static void updateRequest();
      static void dataUpload();
      static void dataDownload();
    } operations;
    struct get
    {
      static String userID();
      static uint32_t gatewayID();
      static uint8_t totalStations();
      static uint8_t status();
      static int32_t station(uint8_t station, uint8_t param);
      static int32_t station(uint8_t station);
      static String token();
      static token_info_t tokenInfo();
    } get;
    struct set
    {
      static void userID(String userID);
      static void gatewayID(uint32_t gatewayID);
      static void station(uint8_t station, uint8_t param, int32_t data);
      static void station(uint8_t station, int32_t data);
      static void timeout(uint8_t fTimeout);
    } set;
  };
}
