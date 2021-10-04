#include "_configBegin.h"

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void configBegin()  //Configurações iniciais
{ 
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  delay(1000);
  Serial.println ("Iniciando...");
  delay(1000); 
}
