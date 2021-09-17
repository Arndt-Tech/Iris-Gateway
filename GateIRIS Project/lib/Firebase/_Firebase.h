#ifndef _FIREBASE_H
#define _FIREBASE_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <FirebaseESP32.h>
//#include "addons/TokenHelper.h "
//#include "addons/RTDBHelper.h"

// Definições
#define FIREBASE_HOST "iris-24c17-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZQlCp7oKTm7QYupuhh9b2C8pqajd3CoxGwK9iAbl"

#define INIT_MAX_STATIONS 128
#define MAX_STATIONS 5
#define CENTER_ID_PREPROCESS "/Users/" + fb->USER_ID + "/Gateway/" + fb->GATEWAY_ID + "/Station/"
//#define CENTER_ID_PREPROCESS "/Users/" + fb->USER_ID + "/Gateway/" + fb->GATEWAY_ID + "/Station/"
// Struct's
typedef struct firestruct
{
  String CENTER_ID;
  String USER_ID;
  String GATEWAY_ID;
  String STATION_ID[INIT_MAX_STATIONS];
  uint16_t aux_TOTAL_STATIONS;
  uint16_t TOTAL_STATIONS;
  FirebaseData FIREBASE_DATA;
} networkFirebase;

// Funções
void setupFirebase(networkFirebase *fb);
bool readStation(networkFirebase *fb);

#endif
