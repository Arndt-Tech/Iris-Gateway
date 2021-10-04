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
  delay(3000);
}

void dataBar(String icon, uint8_t stations, bool commit, bool clear)
{
  if (clear)
    display.clear();
  display.drawHorizontalLine(0, 21, 128);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(0, 4, String(stations));
  display.drawIco16x16(10, 2, station, false);

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(Meteocons_Regular_18);
  display.drawString(128, 0, icon);
  
  if (commit)
    display.display();
}

void runnigSystem(networkLora *gtw, bool commit, bool clear)
{
  if (clear)
    display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(0, 25, "Gateway: ");
  display.drawString(0, 45, "Local: " + String(gtw->localAddr));
  if (commit)
    display.display();
}