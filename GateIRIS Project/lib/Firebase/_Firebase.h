#ifndef _FIREBASE_H
#define _FIREBASE_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <FirebaseESP32.h>

// Definições
#define FIREBASE_HOST "iris-24c17-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZQlCp7oKTm7QYupuhh9b2C8pqajd3CoxGwK9iAbl"
// Locais
#define CENTER_ID_PREPROCESS "/Users/" + fb->USER_ID + "/Gateway/" + fb->GATEWAY_ID + "/Station/"
#define CENTER_ISON_PREPROCESS "/Users/" + fb->USER_ID + "/Gateway/" + fb->GATEWAY_ID + "/Station/" + value.value + "/isOn/"
#define CENTER_ISCONN_PREPROCESS "/Users/" + fb->USER_ID + "/Gateway/" + fb->GATEWAY_ID + "/Station/" + *fb->STATION_ID[i] + "/isConnected/"
// Inicializações
#define INIT_MAX_STATIONS 10
#define MAX_STATIONS ((INIT_MAX_STATIONS)/2)
#define INIT_PARAMETERS 10
// Parâmetros
#define RETURN 1
#define ISON 2
#define ISCONNECTED 3
#define FB_HUMIDITY 4
#define FB_TEMPERATURE 5

// Struct's
typedef struct firestruct
{
  // ID's
  String USER_ID;
  String GATEWAY_ID;
  String STATION_ID[INIT_MAX_STATIONS][INIT_PARAMETERS];
  // Gerais
  uint16_t TOTAL_STATIONS;
} networkFirebase;

// Funções
void setupFirebase(networkFirebase *fb);
bool readStation(networkFirebase *fb);

#endif
