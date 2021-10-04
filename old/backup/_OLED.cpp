#include "_OLED.h"

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupOLED()
{
  pinMode (OLED_RST, OUTPUT);
  digitalWrite (OLED_RST, 0);
  digitalWrite (OLED_RST, 1);
  
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void interfaceDebugLora()
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString (64,  2, "GateIRIS");
  display.drawString (64, 12, "N Pacotes: " + String(pacotes));
  display.drawString (64, 22, "Erros: " + String(erros));
  display.drawString (64, 32, "Tamanho do pacote: " + String(incomingLength));
  display.drawString (64, 42, String(LoRa.packetRssi()) + "dBm");
  display.display();
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void interfacePacotesLora()
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString (64,  2, "GateIRIS");
  display.drawString (64, 12, "Enviando: " + String(valve));
  display.drawString (64, 22, "Recebido de: 0x" + String(sender, HEX));
  display.drawString (64, 32, "Enviado para: 0x" + String(requisitor, HEX));;
  display.drawString (64, 42, "PR: " + incoming);
  display.display();
}
