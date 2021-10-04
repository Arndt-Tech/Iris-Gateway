#ifndef _LORA_H
#define _LORA_H

// LoRa
  #include <LoRa.h>

// SPI
  #include <SPI.h>

// LoRa
  #define INTERVAL 2000
  #define BAND 433E6    // Frequencia 433MHz

// LoRa pinout
  #define SCK   5
  #define MISO 19
  #define MOSI 27
  #define SS   18
  #define RST  14
  #define DI00 26

// LoRa
  void setupLoRa();
  void runningLoRa();
  void send_LoRa_Message(String dados);
  void receive_LoRa_Message(int packetSize);


// Protocolo de envio LoRa
  byte localAddress = 0xBB;     // Endereço deste gateway
  byte comLine = 0xFF;          // Linha de comunicação
  int requisitor;
  byte sender;
  byte incomingLength;
  String incoming = ""; 
  int erros, pacotes;

#endif
