#include "main.hpp"

gtw::Gateway gateway;

void setup()
{
#if _DEBUG_MODE_
  Serial.begin(115200);
  Serial.println("********************************");
  Serial.println("********** DEBUG MODE **********");
  Serial.println("********************************");
#elif _DEGUB_MODE_
#endif

  xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));

  gateway.begin();

#if _DEBUG_MODE_
  Serial.println("Token Info:");
  gateway.Additional().viewTokenInfo(gateway.manage.Firebase().get.tokenInfo());
  Serial.println("Token:");
  Serial.println(gateway.manage.Firebase().get.token());
#elif _DEGUB_MODE_
#endif

  setupTasks();
}

void loop()
{
  gateway.manage.Firebase().operations.updateRequest();
  gateway.manage.Firebase().operations.dataUpload();

#if _DEBUG_MODE_
  Serial.write('\n');
  Serial.write('\n');
  Serial.write('\n');
  Serial.println("****************************************");
  Serial.println("********** Gateway Data Debug **********");
  Serial.println("****************************************");
  Serial.write('\n');
  gateway.manage.LoRa().package.send.get.destinationAddress();
  Serial.write('\n');
  Serial.println("---------------------------------------------------");
  Serial.println("LoRa unit - shipping package");
  Serial.write('\n');
  Serial.println("Sending to: " + String(gateway.manage.LoRa().package.send.get.destinationAddress()));
  Serial.println("Local Address: " + String(gateway.manage.LoRa().package.send.get.localAddress()));
  Serial.print("Valve Status: ");
  Serial.println(gateway.manage.LoRa().package.send.get.valveStatus() == true ? "On" : "Off");
  Serial.println("Package size: " + String(gateway.manage.LoRa().package.send.get.size())) + " Bytes";
  Serial.write('\n');
  Serial.println("---------------------------------------------------");
  Serial.println("Lora unit - receiving package");
  Serial.write('\n');
  Serial.println("Sender: " + String(gateway.manage.LoRa().package.receive.get.senderAddress()));
  Serial.println("Recipient: " + String(gateway.manage.LoRa().package.receive.get.receiveAddress()));
  Serial.println("Station Number: " + String(gateway.manage.LoRa().package.receive.get.stationNumber()));
  Serial.println("Humidity: " + String(gateway.manage.LoRa().package.receive.get.humidity()));
  Serial.println("Temperature: " + String(gateway.manage.LoRa().package.receive.get.temperature() / 10));
  Serial.println("Latitude: " + String(gateway.manage.LoRa().package.receive.get.latitude() / (-1E6)));
  Serial.println("Longitude: " + String(gateway.manage.LoRa().package.receive.get.longitude() / (-1E6)));
  Serial.println("Package size: " + String(gateway.manage.LoRa().package.receive.get.size()) + " Bytes");
  Serial.println("Signal: " + String(gateway.manage.LoRa().package.receive.get.signal()));
  Serial.write('\n');
  Serial.println("---------------------------------------------------");
  Serial.println("Wifi unit");
  Serial.write('\n');
  Serial.println("SSID: " + String(gateway.manage.Wifi().get.SSID()));
  Serial.println("Signal: " + String(gateway.manage.Wifi().get.signal()));
  Serial.println("Status: " + gateway.Additional().WiFiStatus());
  Serial.println("Local IP: " + String(gateway.manage.Wifi().get.IP()));
  Serial.println("Gateway IP: " + String(gateway.manage.Wifi().get.gatewayIP()));
  Serial.println("DNS IP: " + String(gateway.manage.Wifi().get.dnsIP()));
  Serial.println("Mac Address: " + String(gateway.manage.Wifi().get.macAddress()));
  Serial.println("Subnet Mask: " + String(gateway.manage.Wifi().get.subnetMask()));
  Serial.write('\n');
  Serial.println("---------------------------------------------------");
  Serial.println("Firebase unit");
  Serial.write('\n');
  Serial.println("User ID: " + String(gateway.manage.Firebase().get.userID()));
  Serial.println("Gateway ID: " + String(gateway.manage.Firebase().get.gatewayID()));
  Serial.println("Total Stations: " + String(gateway.manage.Firebase().get.totalStations()));
  Serial.println("Firebase status: " + String(gateway.manage.Firebase().get.status()));
  Serial.print("Token Info: ");
  gateway.Additional().viewTokenInfo(gateway.manage.Firebase().get.tokenInfo());
  Serial.println("Token: ");
  Serial.println(gateway.manage.Firebase().get.token());
  Serial.write('\n');
  Serial.println("---------------------------------------------------");
  Serial.println("Bluetooth unit");
  Serial.write('\n');
  Serial.print("Connection Status: ");
  Serial.println(gateway.manage.Bluetooth().getConnectionStatus() == true ? "Connected" : "Disconnected");
  Serial.println("---------------------------------------------------");
  Serial.write('\n');
  vTaskDelay(5000);
#elif
#endif
}
