#include "OLED.h"

// Instâncias
SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

// Funções
void setupOLED()
{
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, 0);
  digitalWrite(OLED_RST, 1);
  display.init();
  display.flipScreenVertically();

  display.clear();
  display.drawRect(0, 0, 126, 62);
  display.drawXbm(48, 7, logo_widht, logo_height, logo);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_12);
  display.drawString(64, 41, "GateIRIS");
  display.display();
  vTaskDelay(3000);
}

void dataBar(networkBluetooth *ble, networkWiFi *wifi, networkFirebase *fb, networkLora *gtw, bool commit, bool clear)
{
  if (clear)
    display.clear();
  display.drawHorizontalLine(0, 21, 128);
  if (wifi->SIGNAL > -50 && WiFi.status() == WL_CONNECTED)
    display.drawIco16x16(111, 1, wifi_high_signal, false);
  else if (wifi->SIGNAL < -50 && wifi->SIGNAL >= -60 && WiFi.status() == WL_CONNECTED)
    display.drawIco16x16(111, 1, wifi_mid_high_signal, false);
  else if (wifi->SIGNAL < -60 && wifi->SIGNAL >= -70 && WiFi.status() == WL_CONNECTED)
    display.drawIco16x16(111, 1, wifi_mid_low_signal, false);
  else if (wifi->SIGNAL < -70 && WiFi.status() == WL_CONNECTED)
    display.drawIco16x16(111, 1, wifi_low_signal, false);
  else if (WiFi.status() != WL_CONNECTED)
    display.drawIco16x16(111, 1, wifi_not_signal, false);
  if (fb->STATUS || WiFi.status() == WL_CONNECTED)
    display.drawIco16x16(85, 2, server_connected, false);
  else
    display.drawIco16x16(85, 2, server_disconnected, false);
  if (ble->status)
    display.drawIco16x16(59, 2, bluetooth_on, false);
  else
    display.drawIco16x16(59, 2, bluetooth_off, false);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(0, 4, String(fb->TOTAL_STATIONS));
  display.drawIco16x16(10, 2, station, false);
  if (commit)
    display.display();
}

void runnigSystem(networkLora *gtw, bool commit, bool clear)
{
  if (clear)
    display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_12);
  display.drawString(0, 25, "    Heap: " + String(xPortGetFreeHeapSize()));
  //display.drawString(0, 45, "Local: " + String(gtw->sendPacket.localAddr));
  if (commit)
    display.display();
}