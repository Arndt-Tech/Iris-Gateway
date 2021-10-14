// Inclusões
#include "_Firebase.h"

void setupFirebase(networkFirebase *fb)
{
  Serial.println("Inicializando Firebase");
  fb->config.database_url = FIREBASE_HOST;
  fb->config.api_key = FIREBASE_API;
  fb->auth.user.email = "danielarndt959@gmail.com";
  fb->auth.user.password = "tst123";
  Firebase.begin(&fb->config, &fb->auth);
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

bool readStation(networkFirebase *fb)
{
  if (Firebase.ready())
  {
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
          Firebase.RTDB.getBool(&fb->FIREBASE_DATA, CENTER_ISON_RTDB, &fb->STATION_ID[st][ISON]);
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
  if (Firebase.ready())
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
    static unsigned long tFSDB;
    static uint8_t stationCursor = 0;
    if ((xTaskGetTickCount() - tFSDB) > TIME(5))
    {
      tFSDB = xTaskGetTickCount();
      uint16_t __temp = fb->STATION_ID[stationCursor][FB_TEMPERATURE];
      double __latitude = fb->STATION_ID[stationCursor][FB_LATITUDE];
      double __longitude = fb->STATION_ID[stationCursor][FB_LONGITUDE];
      FirebaseJson content;
      String documentPath = CENTER_COLLECTION;
      std::vector<struct fb_esp_firestore_document_write_t> writes;
      struct fb_esp_firestore_document_write_t update_write;
      update_write.type = fb_esp_firestore_document_write_type_update;
      content.set("fields/temperature/doubleValue", (float)__temp / 10);
      content.set("fields/humidity/integerValue", fb->STATION_ID[stationCursor][FB_HUMIDITY]);
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
      stationCursor += 1;
      if (stationCursor >= fb->TOTAL_STATIONS)
        stationCursor = 0;
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
// Teste commit documment
void teste(networkFirebase *fb)
{
  static unsigned long dataMillis;
  int count = 0;
  if (Firebase.ready() && (millis() - dataMillis > 60000 || dataMillis == 0))
  {
    //The dyamic array of write object fb_esp_firestore_document_write_t.
    std::vector<struct fb_esp_firestore_document_write_t> writes;
    //A write object that will be written to the document.
    struct fb_esp_firestore_document_write_t update_write;
    //Set the write object write operation type.
    //fb_esp_firestore_document_write_type_update,
    //fb_esp_firestore_document_write_type_delete,
    //fb_esp_firestore_document_write_type_transform
    update_write.type = fb_esp_firestore_document_write_type_update;
    //Set the document content to write (transform)
    FirebaseJson content;
    String documentPath = "/Users/Lco9IpTQPTZhwnCjz4xmFBwwgUt1/Gateway/6038996/Station/6038296/data/";
    content.set("fields/temperature/doubleValue", 27.3);

    //Set the update document content
    update_write.update_document_content = content.raw();
    //Set the update document path
    update_write.update_document_path = documentPath.c_str();
    //Set the document mask field paths that will be updated
    //Use comma to separate between the field paths
    update_write.update_masks = "temperature";
    writes.push_back(update_write);
    if (Firebase.Firestore.commitDocument(&fb->FIREBASE_DATA, PROJECT_ID, "", writes, ""))
      Serial.printf("ok\n%s\n\n", fb->FIREBASE_DATA.payload().c_str());
    else
      Serial.println(fb->FIREBASE_DATA.errorReason());
  }
}

//Teste create
dataMillis = millis();

    FirebaseJson content;

    //We will create the nested document in the parent path "a0/b0/c0
    //a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
    //and d? is the document id in the document collection id c0 which we will create.
    String documentPath = "a0/b0/c0/d" + String(count);

    //If the document path contains space e.g. "a b c/d e f"
    //It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

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

    count++;

    Serial.print("Create a document... ");

    if (Firebase.Firestore.createDocument(&fb->FIREBASE_DATA, PROJECT_ID, "" , documentPath.c_str(), content.raw()))
      Serial.printf("ok\n%s\n\n", fb->FIREBASE_DATA.payload().c_str());
    else Serial.println(fb->FIREBASE_DATA.errorReason());
*/