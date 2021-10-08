#ifndef _LORA_H
#define _LORA_H

//InclusÃµes
#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>
#include "_Firebase.h"

// Definições
#define STD 0
#define DELETE 2

// LoRa Config.
#define INTERVAL 2000 // 2000 + 205ms para compensar o delay de processamento do Gateway
#define BAND 433E6    // Frequencia 433MHz
// Pinout
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI00 26

// Definições
#define loraTmt 10000

// Struct's
typedef struct _lora
{
  uint32_t localAddr;
  uint8_t stationCursor;
  uint8_t isOn;
  int8_t packSize;
} networkLora;

typedef struct
{
  uint8_t dest_addr[4], sender_addr[4];
  uint8_t fSender, iterator;
  uint8_t aux_hmdt, aux_temp[2];
  uint16_t packageLength;
} _loraData;

// Funções
void setupLoRa(networkLora *gtw);
void runningLoRa(networkLora *gtw, networkFirebase *fb);
void send_LoRa_Message(networkLora *gtw, networkFirebase *fb);
String receive_LoRa_Message(networkLora *gtw, networkFirebase *fb);
void org_FB_data(_loraData *__data, networkFirebase *fb);
void verify_LoRa_Timeout(networkFirebase *fb);
uint32_t asm_addr(uint8_t *addr);

#endif