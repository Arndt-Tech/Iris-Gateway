#pragma once

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>

// Tamanho padrão dos ícones 
#define icon_16x 16  // 16x16

// Ícones
const uint8_t wifi_high_signal[] PROGMEM = 
{
  0x1E, 0x00, 0xE0, 0x00, 0x80, 0x03, 0x0E, 0x06, 0x70, 0x0C, 0xC0, 0x18, 
  0x04, 0x31, 0x3C, 0x22, 0x60, 0x64, 0xC0, 0x4C, 0x8E, 0x49, 0x10, 0x89, 
  0x20, 0x91, 0x27, 0x93, 0x27, 0x90, 0x06, 0x00,
};

const uint8_t wifi_mid_high_signal[] PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x70, 0x00, 0xC0, 0x00, 
  0x04, 0x01, 0x3C, 0x02, 0x60, 0x04, 0xC0, 0x0C, 0x8E, 0x09, 0x10, 0x09, 
  0x20, 0x11, 0x27, 0x13, 0x27, 0x10, 0x06, 0x00, 
};

const uint8_t wifi_mid_low_signal[] PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x04, 0x00, 0x3C, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x8E, 0x01, 0x10, 0x01, 
  0x20, 0x01, 0x27, 0x03, 0x27, 0x00, 0x06, 0x00, 
};

const uint8_t wifi_low_signal[] PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x10, 0x00, 
  0x20, 0x00, 0x27, 0x00, 0x27, 0x00, 0x06, 0x00, 
};

const uint8_t wifi_not_signal[] PROGMEM = 
{
  0x1E, 0x00, 0xE0, 0xC6, 0x80, 0x7C, 0x0E, 0x38, 0x70, 0x7C, 0xC0, 0xC6, 
  0x04, 0x01, 0x3C, 0x02, 0x60, 0x64, 0xC0, 0x4C, 0x8E, 0x49, 0x10, 0x89, 
  0x20, 0x91, 0x27, 0x93, 0x27, 0x90, 0x06, 0x00, 
};

const uint8_t station[] PROGMEM =
{
  0x00, 0x00, 0xF0, 0x0F, 0x50, 0x0A, 0xF0, 0x0F, 0x50, 0x0A, 0xF0, 0x0F, 
  0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x03, 0x40, 0x02, 
  0xE0, 0x03, 0x20, 0x04, 0xE0, 0x07, 0x20, 0x04,
};

const uint8_t station2[] PROGMEM = 
{
  0x00, 0x00, 0xF0, 0x0F, 0x50, 0x0A, 0xF0, 0x0F, 0x50, 0x0A, 0xF0, 0x0F, 
  0x80, 0x01, 0x80, 0x01, 0xC0, 0x03, 0xE0, 0x07, 0x30, 0x0C, 0x10, 0x08, 
  0xF8, 0x1F, 0x48, 0x12, 0xF8, 0x1F, 0x08, 0x10,
};

const uint8_t server_connected[] PROGMEM = 
{
  0xC0, 0x03, 0x70, 0x06, 0x18, 0x18, 0x08, 0x30, 0x0C, 0x60, 0x06, 0xC0, 
  0x03, 0x80, 0x01, 0x80, 0x01, 0x80, 0x23, 0xC0, 0xF6, 0x37, 0x20, 0x00, 
  0x00, 0x04, 0xE0, 0x0F, 0x00, 0x04, 0x00, 0x00,
};

const uint8_t server_disconnected[] PROGMEM = 
{
  0xC0, 0x03, 0x70, 0x06, 0x18, 0x18, 0x08, 0x30, 0x0C, 0x60, 0x06, 0xC0, 
  0x03, 0x80, 0x11, 0x84, 0x31, 0x86, 0x63, 0xC3, 0xC6, 0x71, 0xC0, 0x01, 
  0x60, 0x03, 0x30, 0x06, 0x10, 0x04, 0x00, 0x00,
};

const uint8_t bluetooth_on[] PROGMEM = 
{
  0x40, 0x00, 0xC0, 0x00, 0xC0, 0x01, 0xC6, 0x03, 0x4C, 0x27, 0x78, 0x63, 
  0xF0, 0x61, 0xE0, 0x4C, 0xE0, 0x4C, 0xF0, 0x61, 0x78, 0x63, 0x4C, 0x27, 
  0xC6, 0x03, 0xC0, 0x01, 0xC0, 0x00, 0x40, 0x00,
};

const uint8_t bluetooth_off[] PROGMEM = 
{
  0x80, 0x01, 0x80, 0x03, 0x8C, 0x07, 0x9C, 0x0F, 0x38, 0x0D, 0x70, 0x0E, 
  0xE0, 0x04, 0xC0, 0x01, 0xC0, 0x03, 0xE0, 0x07, 0xF0, 0x0F, 0xB8, 0x1D, 
  0x98, 0x3F, 0x80, 0x37, 0x80, 0x03, 0x80, 0x01,
};
 