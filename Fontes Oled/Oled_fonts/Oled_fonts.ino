#include "fonts.h"

#include <SSD1306.h>

// SSD1306 pinout
  #define OLED_SCL 15
  #define OLED_SDA 4
  #define OLED_RST 16

SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

void setupOLED();
void waitingStart();


void setup() 
{
  Serial.begin(115200);
  setupOLED();
}

void loop() 
{
  
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupOLED()
{
  pinMode (OLED_RST, OUTPUT);
  digitalWrite (OLED_RST, 0);
  digitalWrite (OLED_RST, 1);
  
  display.init();
  display.flipScreenVertically();
  display.setFont(Aclonica_Regular_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingStart()
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString (64, 22, "Esperando conexão...");
  display.display();
}
