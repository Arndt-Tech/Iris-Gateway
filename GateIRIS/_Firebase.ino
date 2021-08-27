#include "_Firebase.h"

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupFirebase()
{
  estacao = String(debugID);
  //temperatura = String (tempID);
  //umidade = String (umidID);
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  Firebase.set(firebaseData, estacao, "Desligado");
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void runningFirebase()
{
  Firebase.get(firebaseData, estacao);
  fireString = firebaseData.stringData();
  if (fireString == "Ligado")valve = 1;
  else if (fireString == "Desligado")valve = 0;
  //Serial.println ("Heap depois firebase: " +  String(xPortGetFreeHeapSize()));
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void runningFirebaseTemp()
{
  Firebase.set(firebaseData, temperatura, temperature);
  Firebase.set(firebaseData, umidade, humidity);
}
