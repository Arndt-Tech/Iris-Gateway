// Inclusões
#include "_Firebase.h"

void setupFirebase(networkFirebase *fb)
{
  Serial.println("Inicializando Firebase");
  fb->CONFIG.database_url = FIREBASE_HOST;
  fb->CONFIG.api_key = FIREBASE_API;
  fb->AUTH.user.email = "danielarndt959@gmail.com";
  fb->AUTH.user.password = "tst123";
  Firebase.begin(&fb->CONFIG, &fb->AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Gerando token");
  while (!Firebase.ready())
  {
    Serial.print(".");
    delay(500);
  }
  tokenStatus(Firebase.authTokenInfo());
  tokenType(Firebase.authTokenInfo());
  if (Firebase.ready())
    Serial.println("Token: \n" + String(Firebase.getToken()) + "\n");
  readStation(fb);
}

uint8_t readStation(networkFirebase *fb)
{
  static uint8_t init = false;
  if (Firebase.ready() || !init)
  {
    init = true;
    Firebase.RTDB.setQueryIndex(&fb->FIREBASE_IDS, CENTER_ID_RTDB, "", "");
    if (!Firebase.RTDB.getJSON(&fb->FIREBASE_IDS, CENTER_ID_RTDB))
    {
      Serial.println("Error -> " + fb->FIREBASE_IDS.errorReason());
      return 0;
    }
    FirebaseJson *json = fb->FIREBASE_IDS.to<FirebaseJson *>();
    size_t len = json->iteratorBegin();
    uint16_t st = 0;
    FirebaseJson::IteratorValue value;
    uint16_t aux_TOTAL_STATIONS = fb->TOTAL_STATIONS;
    for (size_t i = 0; i < len; i++)
    {
      value = json->valueAt(i);
      if (value.key == "chipID")
      {
        if (st < MAX_STATIONS)
        {
          *fb->STATION_ID[st] = atol(value.value.c_str());
          st += 1;
        }
        else
          break;
      }
    }
    fb->TOTAL_STATIONS = st;
    if (aux_TOTAL_STATIONS > fb->TOTAL_STATIONS)
    {
      *fb->STATION_ID[fb->TOTAL_STATIONS] = 0;
      for (uint8_t i = 0; i < INIT_PARAMETERS; i++)
        fb->STATION_ID[fb->TOTAL_STATIONS][i] = 0;
    }
    json->iteratorEnd();
    json->clear();
    return 1;
  }
  return 0;
}

void setStatus(networkFirebase *fb)
{
  if (Firebase.ready() && fb->TOTAL_STATIONS > 0)
  {
    if (fb->TIMEOUT)
    {
      fb->TIMEOUT = 0;
      for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
        if (!fb->STATION_ID[i][ISCONNECTED])
          Firebase.RTDB.setBool(&fb->FIREBASE_DATA, CENTER_ISCONN_RTDB, false);
    }
    else
      for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
        if (fb->STATION_ID[i][ISCONNECTED])
          Firebase.RTDB.setBool(&fb->FIREBASE_DATA, CENTER_ISCONN_RTDB, true);
  }
}

void firestoreWrite(networkFirebase *fb)
{
  if (Firebase.ready() && fb->TOTAL_STATIONS > 0)
  {
    static unsigned long tFSDB = 0;
    if ((xTaskGetTickCount() - tFSDB) > TIME(1))
    {
      tFSDB = xTaskGetTickCount();
      for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
      {
        if (fb->STATION_ID[i][ISCONNECTED])
        {
          float __temp = fb->STATION_ID[i][FB_TEMPERATURE];
          double __latitude = fb->STATION_ID[i][FB_LATITUDE];
          double __longitude = fb->STATION_ID[i][FB_LONGITUDE];
          FirebaseJson content;
          String documentPath = CENTER_COLLECTION;
          std::vector<struct fb_esp_firestore_document_write_t> writes;
          struct fb_esp_firestore_document_write_t update_write;
          update_write.type = fb_esp_firestore_document_write_type_update;
          content.set("fields/temperature/doubleValue", __temp / 10);
          content.set("fields/humidity/integerValue", fb->STATION_ID[i][FB_HUMIDITY]);
          content.set("fields/latLong/geoPointValue/latitude", __latitude / -1000000);
          content.set("fields/latLong/geoPointValue/longitude", __longitude / -1000000);
          update_write.update_document_content = content.raw();
          update_write.update_document_path = documentPath.c_str();
          update_write.update_masks = "temperature,humidity,latLong";
          writes.push_back(update_write);
          if (Firebase.Firestore.commitDocument(&fb->FIREBASE_DATA, PROJECT_ID, "", writes, ""))
            Serial.printf("ok\n%s\n\n", fb->FIREBASE_DATA.payload().c_str());
          else
            Serial.println(fb->FIREBASE_DATA.errorReason());
        }
      }
    }
  }
}

void readStatus(networkFirebase *fb)
{
  static uint8_t stationCursor = 0;
  if (Firebase.ready() && fb->TOTAL_STATIONS > 0)
  {
    Firebase.RTDB.getBool(&fb->FIREBASE_DATA, CENTER_REFRESH_STATIONS_RTDB, &fb->REFRESH_STATIONS);
    Firebase.RTDB.getBool(&fb->FIREBASE_DATA, CENTER_ISON_RTDB, &fb->STATION_ID[stationCursor][ISON]);
    stationCursor += 1;
    if (stationCursor >= fb->TOTAL_STATIONS)
      stationCursor = 0;
    if (fb->REFRESH_STATIONS)
    {
      Serial.println("Refreshing");
      if (readStation(fb))
      {
        Firebase.RTDB.setBool(&fb->FIREBASE_DATA, CENTER_REFRESH_STATIONS_RTDB, false);
        fb->REFRESH_STATIONS = 0;
        Serial.println("Refreshed");
      }
      else
        Serial.println("Not refreshed");
    }
  }
}

void tokenStatus(token_info_t token)
{
  switch (token.status)
  {
  case token_status_uninitialized:
    Serial.println("Token nao inicializado.");
    break;

  case token_status_on_initialize:
    Serial.println("Token inicializando.");
    break;

  case token_status_on_signing:
    Serial.println("Token em assinatura.");
    break;

  case token_status_on_request:
    Serial.println("Token em solicitacao.");
    break;

  case token_status_on_refresh:
    Serial.println("Token atualizando.");
    break;

  case token_status_ready:
    Serial.println("Token pronto.");
    break;

  case token_status_error:
    Serial.println("Erro ao inicializar token.");
    break;
  }
}

void tokenType(token_info_t token)
{
  switch (token.type)
  {
  case token_type_undefined:
    Serial.println("Token de tipo indefinido.");
    break;

  case token_type_legacy_token:
    Serial.println("Token de tipo legado.");
    break;

  case token_type_id_token:
    Serial.println("Token de tipo ID.");
    break;

  case token_type_custom_token:
    Serial.println("Token de tipo customizado.");
    break;

  case token_type_oauth2_access_token:
    Serial.println("Token de tipo OAuth2.0.");
    break;

  case token_type_refresh_token:
    Serial.println("Token de tipo atualizacao.");
    break;
  }
}

/*
// Leitura firestore
void firestoreGet(networkFirebase *fb)
{
  static uint8_t stationCursor = 0;
  if (Firebase.ready() && fb->TOTAL_STATIONS > 0)
  {
    String documentPath = CENTER_COLLECTION_CMD;
    String mask = "isOn";
    Serial.print("Get a document... ");
    if (Firebase.Firestore.getDocument(&fb->FIREBASE_DATA, PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
      Serial.printf("ok\n%s\n\n", fb->FIREBASE_DATA.payload().c_str());
    else
      Serial.println(fb->FIREBASE_DATA.errorReason());
    stationCursor += 1;
    if (stationCursor >= fb->TOTAL_STATIONS)
      stationCursor = 0;
  }
}

// Dados firestore
    //double
    content.set("fields/myDouble/doubleValue", 123.45678);

    //boolean
    content.set("fields/myBool/booleanValue", true);

    //integer
    content.set("fields/myInteger/integerValue", "911");

    //null
    content.set("fields/myNull/nullValue"); // no value set

    String doc_path = "projects/";
    doc_path += PROJECT_ID;
    doc_path += "/databases/(default)/documents/coll_id/doc_id"; //coll_id and doc_id are your collection id and document id

    //reference
    content.set("fields/myRef/referenceValue", doc_path.c_str());

    //timestamp
    content.set("fields/myTimestamp/timestampValue", "2014-10-02T15:01:23Z"); //RFC3339 UTC "Zulu" format

    //bytes
    content.set("fields/myBytes/bytesValue", "aGVsbG8="); //base64 encoded

    //array
    content.set("fields/myArray/arrayValue/values/[0]/stringValue", "test");
    content.set("fields/myArray/arrayValue/values/[1]/integerValue", "20");
    content.set("fields/myArray/arrayValue/values/[2]/booleanValue", true);

    //map
    content.set("fields/myMap/mapValue/fields/name/stringValue", "wrench");
    content.set("fields/myMap/mapValue/fields/mass/stringValue", "1.3kg");
    content.set("fields/myMap/mapValue/fields/count/integerValue", "3");

    //lat long
    content.set("fields/myLatLng/geoPointValue/latitude", 1.486284);
    content.set("fields/myLatLng/geoPointValue/longitude", 23.678198);
*/