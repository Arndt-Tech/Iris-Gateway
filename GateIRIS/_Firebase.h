#ifndef _FIREBASE_H
#define _FIREBASE_H

// Firebase
  #include <FirebaseESP32.h> 

// Firebase 
  #define FIREBASE_HOST "iris-24c17-default-rtdb.firebaseio.com"  
  #define FIREBASE_AUTH "ZQlCp7oKTm7QYupuhh9b2C8pqajd3CoxGwK9iAbl" 

  #define valveID   "/Users/" + USER_ID_STR + "/Estacao/" + STATION_ID_STR + "/Status/" + "/StatusApp"
  #define  tempID   "/Users/" + USER_ID_STR + "/Estacao/" + STATION_ID_STR + "/Dados/" + "/Temperatura"
  #define  umidID   "/Users/" + USER_ID_STR + "/Estacao/" + STATION_ID_STR + "/Dados/" + "/Umidade"
  #define debugID   "/Users/Lco9IpTQPTZhwnCjz4xmFBwwgUt1/Estacao/-MdmxU2TikF-dX9Fcxct/Status/StatusApp"

// Firebase
  void setupFirebase();
  void runningFirebase();
  void runningFirebaseTemp();

// Objetos
  // Firebase
  FirebaseData firebaseData; 


//Endereçamento do Firebase 
  String estacao = String("/Users/" + USER_ID_STR + "/Estacao/" + STATION_ID_STR + "/Status/" + "/StatusApp");
  String temperatura = String ("/Users/" + USER_ID_STR + "/Estacao/" + STATION_ID_STR + "/Dados/" + "/Temperatura");
  String umidade = String ("/Users/" + USER_ID_STR + "/Estacao/" + STATION_ID_STR + "/Dados/" + "/Umidade");

// String de dados do Firebase
  String fireString = "";

// timer firebase
   unsigned long tFirebase;  

// Firebase / LoRa Data
  String humidity = "0", temperature = "0";
  bool valve;

#endif
