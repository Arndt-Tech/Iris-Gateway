#ifndef _FIREBASE_H
#define _FIREBASE_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Firebase_ESP_Client.h>

// Definições
#define FIREBASE_HOST "iris-24c17-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZQlCp7oKTm7QYupuhh9b2C8pqajd3CoxGwK9iAbl"
#define FIREBASE_API "AIzaSyDO-jkFKJze7Fz_z6cxps5kBQ4fMiwJa6s"
#define PROJECT_ID "iris-24c17"
// Json - RTDB
#define CENTER_REFRESH_STATIONS_RTDB "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/refresh/"
#define CENTER_ID_RTDB "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/Station/"
#define CENTER_ISON_RTDB "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/Station/" + String(*fb->STATION_ID[stationCursor]) + "/isOn/"
#define CENTER_ISCONN_RTDB "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/Station/" + String(*fb->STATION_ID[i]) + "/isConnected/"
#define CENTER_RSSI_RTDB_CONN "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/Station/" + String(*fb->STATION_ID[stationCursor]) + "/rssi/"
#define CENTER_RSSI_RTDB_DISCONN "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/Station/" + String(*fb->STATION_ID[i]) + "/rssi/"
// Json - Firestore
#define CENTER_COLLECTION "/Users/" + String(fb->USER_ID) + "/Gateway/" + String(fb->GATEWAY_ID) + "/Station/" + String(*fb->STATION_ID[i])
// Inicializações
#define INIT_MAX_STATIONS 10
#define MAX_STATIONS ((INIT_MAX_STATIONS) / 2)
#define INIT_PARAMETERS 10
// Parâmetros
#define RETURN 1
#define ISON 2
#define ISCONNECTED 3
#define STATION_SIGNAL 4
#define FB_HUMIDITY 5
#define FB_TEMPERATURE 6
#define FB_LATITUDE 7
#define FB_LONGITUDE 8
// Outros
#define TIME(a)  (float)((a) * 60000)

    // Struct's
    typedef struct firestruct
{
  // Configurações
  FirebaseConfig CONFIG;
  FirebaseAuth AUTH;
  // Dados
  FirebaseData FIREBASE_IDS;
  FirebaseData FIREBASE_DATA;
  // ID's
  String USER_ID;
  uint32_t GATEWAY_ID;
  int32_t STATION_ID[INIT_MAX_STATIONS][INIT_PARAMETERS];
  // Gerais
  uint8_t TOTAL_STATIONS;
  // Operacional
  uint8_t REFRESH_STATIONS;
  uint8_t STATUS;
  uint8_t TIMEOUT;
} networkFirebase;

// Funções
void setupFirebase(networkFirebase *fb);
uint8_t readStation(networkFirebase *fb);
void setStatus(networkFirebase *fb);
void firestoreWrite(networkFirebase *fb);
void readStatus(networkFirebase *fb);

void tokenStatus(token_info_t token);
void tokenType(token_info_t token);

#endif
