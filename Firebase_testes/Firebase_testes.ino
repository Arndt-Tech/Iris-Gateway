/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

//This example shows how to construct queries to filter data.

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

void readStation(FirebaseData &data);

/* 1. Define the WiFi credentials */
#define WIFI_SSID "VIVOFIBRA-4094"
#define WIFI_PASSWORD "AGecQfiKSc"

#define API_KEY "ZQlCp7oKTm7QYupuhh9b2C8pqajd3CoxGwK9iAbl"
#define DATABASE_URL "iris-24c17-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

FirebaseJson json;

bool taskCompleted = false;

void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(DATABASE_URL, API_KEY);

  Firebase.reconnectWiFi(true);
}

void loop()
{
  if (Firebase.ready() && !taskCompleted)
  {

    taskCompleted = true;
    
    FirebaseJson json;
    for (uint8_t i = 0; i < 3; i++)
    {
      Firebase.set(fbdo, "/Teste/Usuario/Gateway/Gateway_ID/Station/Estacao" + String(i) + "/chipID", 40928443+(i*754));
      Firebase.set(fbdo, "/Teste/Usuario/Gateway/Gateway_ID/Station/Estacao" + String(i) + "/Lixo", ".............");
    }
    Serial.println();
    
    //Add an index to the node that being query.
    //Update the existing database rules by adding key "test/push/.indexOn" and value "Data2"
    //Check your database rules changes after running this function.

    // Seta Índice de consulta
    Firebase.setQueryIndex(fbdo, "/Teste/Usuario/Gateway/Gateway_ID/Station/", "", "");
                                 ///Users/Lco9IpTQPTZhwnCjz4xmFBwwgUt1/Gateway/6038996/Station/
    
    // Verifica se o arquivo filtrado foi encontrado
    Serial.printf("Get json... %s\n", Firebase.getJSON(fbdo, "/Teste/Usuario/Gateway/Gateway_ID/Station/") ? "ok" : fbdo.errorReason().c_str());

    // Pega arquivo filtrado
    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
      readStation(fbdo); //see addons/RTDBHelper.h
  }
}

void readStation(FirebaseData &data)
{
  Firebase.setQueryIndex(fbdo, "/Teste/Usuario/Gateway/Gateway_ID/Station/", "", "");
  if(!Firebase.getJSON(fbdo, "/Teste/Usuario/Gateway/Gateway_ID/Station/") ? "ok" : fbdo.errorReason().c_str())
    return;
  FirebaseJson *json = data.to<FirebaseJson*>();
  size_t len = json->iteratorBegin();
  FirebaseJson::IteratorValue value;
  for (size_t i = 0; i < len; i++)
  {
    value = json->valueAt(i); 
    if (value.key == "chipID")Serial.printf(("Estacao %d -> %s: %s\n"), i, value.key.c_str(), value.value.c_str());
  }
  json->iteratorEnd();
  json->clear();
}
