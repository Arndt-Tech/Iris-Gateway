// Inclusões
#include "_Firebase.h"

void setupFirebase(networkFirebase *fb)
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  readStation(fb);
}

bool readStation(networkFirebase *fb)
{
  if (Firebase.ready())
  {
    FirebaseData FIREBASE_IDS, FIREBASE_DATA;
    Firebase.setQueryIndex(FIREBASE_IDS, CENTER_ID_PREPROCESS, "", "");
    if (!Firebase.getJSON(FIREBASE_IDS, CENTER_ID_PREPROCESS)) //Serial.println("Error -> " + fb->FIREBASE_DATA.errorReason());
      return 0;
    FirebaseJson *json = FIREBASE_IDS.to<FirebaseJson *>();
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
          Firebase.getBool(FIREBASE_DATA, CENTER_ISON_PREPROCESS, &fb->STATION_ID[st][ISON]);
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