// Inclusões
#include "_Firebase.h"

FirebaseData com::FirebaseServer::m_firebase_ids;
FirebaseData com::FirebaseServer::m_firebase_data;
String com::FirebaseServer::m_user_id;
uint32_t com::FirebaseServer::m_gateway_id;
int32_t com::FirebaseServer::m_station[INIT_MAX_STATIONS][INIT_PARAMETERS];
uint8_t com::FirebaseServer::m_total_stations;
uint8_t com::FirebaseServer::m_refresh_stations;
uint8_t com::FirebaseServer::m_sync_refresh;
uint8_t com::FirebaseServer::m_status;
uint8_t com::FirebaseServer::m_timeout;
String com::FirebaseServer::m_token;
token_info_t com::FirebaseServer::m_token_info;

void com::FirebaseServer::begin()
{
  m_gateway_id = spc::SpecialFunctions::getChipID();
  m_config.database_url = FIREBASE_HOST;
  m_config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&m_config, &m_auth);
  Firebase.reconnectWiFi(true);
  if (Firebase.ready())
    m_token = String(Firebase.getToken());
  m_token_info = Firebase.authTokenInfo();
  refreshStations();
}

uint8_t com::FirebaseServer::refreshStations()
{
  static uint8_t init = false;
  if (Firebase.ready() || !init)
  {
    init = true;
    Firebase.RTDB.setQueryIndex(&m_firebase_ids, CENTER_ID_RTDB, "", "");
    if (!Firebase.RTDB.getJSON(&m_firebase_ids, CENTER_ID_RTDB))
    {
      Serial.println("Error -> " + m_firebase_ids.errorReason());
      return 0;
    }
    FirebaseJson *json = m_firebase_ids.to<FirebaseJson *>();
    size_t len = json->iteratorBegin();
    uint16_t st = 0;
    FirebaseJson::IteratorValue value;
    uint16_t aux_TOTAL_STATIONS = m_total_stations;
    for (size_t i = 0; i < len; i++)
    {
      value = json->valueAt(i);
      if (value.key == "chipID")
      {
        if (st < MAX_STATIONS)
        {
          *m_station[st] = atol(value.value.c_str());
          st += 1;
        }
        else
          break;
      }
    }
    m_total_stations = st;
    if (aux_TOTAL_STATIONS > m_total_stations)
    {
      *m_station[m_total_stations] = 0;
      for (uint8_t i = 0; i < INIT_PARAMETERS; i++)
        m_station[m_total_stations][i] = 0;
    }
    json->iteratorEnd();
    json->clear();
    return 1;
  }
  return 0;
}

void com::FirebaseServer::runConnectionStatusSystem()
{
  if (Firebase.ready() && m_total_stations > 0)
  {
    if (m_timeout)
    {
      m_timeout = 0;
      for (uint8_t i = 0; i < m_total_stations; i++)
      {
        if (!m_station[i][ISCONNECTED])
        {
          Firebase.RTDB.setBool(&m_firebase_data, CENTER_ISCONN_RTDB, false);
          Firebase.RTDB.setInt(&m_firebase_data, CENTER_RSSI_RTDB_DISCONN, 0);
        }
      }
    }
    else if (!m_refresh_stations)
    {
      for (uint8_t i = 0; i < m_total_stations; i++)
      {
        if (m_station[i][ISCONNECTED])
        {
          Firebase.RTDB.setBool(&m_firebase_data, CENTER_ISCONN_RTDB, true);
          Firebase.RTDB.setInt(&m_firebase_data, CENTER_RSSI_RTDB_DISCONN, m_station[i][STATION_SIGNAL]);
        }
      }
    }
  }
}

void com::FirebaseServer::opr::updateRequest()
{
  static uint8_t stationCursor = 0;
  m_status = Firebase.authenticated();
  if (Firebase.ready())
  {
    uint8_t __refresh_aux = m_refresh_stations;
    if (!Firebase.RTDB.getBool(&m_firebase_data, CENTER_REFRESH_STATIONS_RTDB, &m_refresh_stations))
      m_refresh_stations = __refresh_aux;
    if (m_refresh_stations)
    {
      if (refreshStations())
      {
        Firebase.RTDB.setBool(&m_firebase_data, CENTER_REFRESH_STATIONS_RTDB, false);
      }
    }
  }
  runConnectionStatusSystem();
  m_refresh_stations = 0;
  stationCursor += 1;
  if (stationCursor >= m_total_stations)
    stationCursor = 0;
}

void com::FirebaseServer::opr::dataUpload()
{
  static unsigned long tUpload = 0;
  if (spc::SpecialFunctions::ctrlTickCount(xTaskGetTickCount(), tUpload) > TIME(20000))
  {
    if (Firebase.ready() && m_total_stations > 0)
    {
      for (register uint8_t i = 0; i < m_total_stations; i++)
      {
        Firebase.RTDB.setFloat(&m_firebase_data, CENTER_TEMPERATURE_RTDB, (double)m_station[i][FB_TEMPERATURE] / 10);
        Firebase.RTDB.setInt(&m_firebase_data, CENTER_HUMIDITY_RTDB, m_station[i][FB_HUMIDITY]);
        Firebase.RTDB.setDouble(&m_firebase_data, CENTER_LATITUDE_RTDB, (double)m_station[i][FB_LATITUDE] / (-1E6));
        Firebase.RTDB.setDouble(&m_firebase_data, CENTER_LONGITUDE_RTDB, (double)m_station[i][FB_LONGITUDE] / (-1E6));
      }
    }
  }
}

void com::FirebaseServer::opr::dataDownload()
{
  static uint8_t init = 0;
  if (Firebase.ready() && m_total_stations > 0)
  {
    if (!init)
    {
      init = 1;
      double lat = 0, lon = 0;
      float temp = 0;
      uint8_t humid = 0;
      for (register uint8_t i = 0; i < m_total_stations; i++)
      {
        if (Firebase.RTDB.getFloat(&m_firebase_data, CENTER_TEMPERATURE_RTDB, &temp))
          m_station[i][FB_TEMPERATURE] = temp * 10;
        if (Firebase.RTDB.getInt(&m_firebase_data, CENTER_HUMIDITY_RTDB, &humid))
          m_station[i][FB_HUMIDITY] = humid;
        if (Firebase.RTDB.getDouble(&m_firebase_data, CENTER_LATITUDE_RTDB, &lat))
          m_station[i][FB_LATITUDE] = lat * 1E6;
        if (Firebase.RTDB.getDouble(&m_firebase_data, CENTER_LONGITUDE_RTDB, &lon))
          m_station[i][FB_LONGITUDE] = lon * 1E6;
      }
    }
    for (register uint8_t i = 0; i < m_total_stations; i++)
    {
      uint8_t __ison_aux = 0;
      uint8_t __ison = (uint8_t)m_station[i][ISON];
      if (!Firebase.RTDB.getBool(&m_firebase_data, CENTER_ISON_RTDB, &__ison_aux))
        m_station[i][ISON] = __ison;
      else
        m_station[i][ISON] = __ison_aux;
    }
  }
}

void com::FirebaseServer::set::userID(String userID) { m_user_id = userID; }

String com::FirebaseServer::get::userID() { return m_user_id; }

void com::FirebaseServer::set::gatewayID(uint32_t gatewayID) { m_gateway_id = gatewayID; }

uint32_t com::FirebaseServer::get::gatewayID() { return m_gateway_id; }

uint8_t com::FirebaseServer::get::totalStations() { return m_total_stations; }

void com::FirebaseServer::set::station(uint8_t station, uint8_t param, int32_t data) { m_station[station][param] = data; }

void com::FirebaseServer::set::station(uint8_t station, int32_t data) { *m_station[station] = data; }

int32_t com::FirebaseServer::get::station(uint8_t station, uint8_t param) { return m_station[station][param]; }

int32_t com::FirebaseServer::get::station(uint8_t station) { return *m_station[station]; }

void com::FirebaseServer::set::timeout(uint8_t fTimeout) { m_timeout = fTimeout; }

uint8_t com::FirebaseServer::get::status() { return m_status; }

String com::FirebaseServer::get::token() { return m_token; }

token_info_t com::FirebaseServer::get::tokenInfo() { return m_token_info; }

/*
void firestoreWrite(networkFirebase *fb)
{
  if (Firebase.ready() &&m_total_stations > 0)
  {
    static unsigned long tFSDB = 0;
    if ((xTaskGetTickCount() - tFSDB) > TIME(5))
    {
      tFSDB = xTaskGetTickCount();
      for (uint8_t i = 0; i <m_total_stations; i++)
      {
        if (m_station[i][ISCONNECTED])
        {
          float __temp = m_station[i][FB_TEMPERATURE];
          double __latitude = m_station[i][FB_LATITUDE];
          double __longitude = m_station[i][FB_LONGITUDE];
          FirebaseJson content;
          String documentPath = CENTER_COLLECTION;
          std::vector<struct fb_esp_firestore_document_write_t> writes;
          struct fb_esp_firestore_document_write_t update_write;
          update_write.type = fb_esp_firestore_document_write_type_update;
          content.set("fields/temperature/doubleValue", __temp / 10);
          content.set("fields/humidity/integerValue", m_station[i][FB_HUMIDITY]);
          if (__latitude != 0.000000)
            content.set("fields/latLong/geoPointValue/latitude", __latitude / -1000000);
          if (__longitude != 0.000000)
            content.set("fields/latLong/geoPointValue/longitude", __longitude / -1000000);
          update_write.update_document_content = content.raw();
          update_write.update_document_path = documentPath.c_str();
          update_write.update_masks = "temperature,humidity,latLong";
          writes.push_back(update_write);
          if (Firebase.Firestore.commitDocument(&m_firebase_data, PROJECT_ID, "", writes, ""))
            Serial.printf("ok\n%s\n\n", m_firebase_data.payload().c_str());
          else
            Serial.println(m_firebase_data.errorReason());
        }
      }
    }
  }
}
*/

/*
// Leitura firestore
void firestoreGet(networkFirebase *fb)
{
  static uint8_t stationCursor = 0;
  if (Firebase.ready() &&m_total_stations > 0)
  {
    String documentPath = CENTER_COLLECTION_CMD;
    String mask = "isOn";
    Serial.print("Get a document... ");
    if (Firebase.Firestore.getDocument(&m_firebase_data, PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
      Serial.printf("ok\n%s\n\n", m_firebase_data.payload().c_str());
    else
      Serial.println(m_firebase_data.errorReason());
    stationCursor += 1;
    if (stationCursor >=m_total_stations)
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