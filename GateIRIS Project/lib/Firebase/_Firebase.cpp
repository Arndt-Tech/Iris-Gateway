// Inclusões
#include "_Firebase.h"

void setupFirebase(networkFirebase *fb)
{
  fb->CENTER_ID = CENTER_ID_PREPROCESS;
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

bool readStation(networkFirebase *fb)
{
  if (Firebase.ready())
  {
    Firebase.setQueryIndex(fb->FIREBASE_DATA, fb->CENTER_ID, "", "");
    if (!Firebase.getJSON(fb->FIREBASE_DATA, fb->CENTER_ID))
    {
      Serial.println("Error -> " + fb->FIREBASE_DATA.errorReason());
      return 0;
    }
    FirebaseJson *json = fb->FIREBASE_DATA.to<FirebaseJson *>();
    size_t len = json->iteratorBegin();
    uint16_t st = 0;
    FirebaseJson::IteratorValue value;
    for (size_t i = 0; i < len; i++)
    {
      value = json->valueAt(i);
      if (value.key == "chipID")
      {
        //Serial.printf(("Estacao %d -> %s: %s\n"), i, value.key.c_str(), value.value.c_str());
        st += 1;
        fb->STATION_ID[i] = value.value;
      }
    }
    fb->TOTAL_STATIONS = st;
    json->iteratorEnd();
    json->clear();
  }
  return 1;
}