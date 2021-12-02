#include "Gateway.hpp"

gtw::Gateway gateway;

void setup()
{
  gateway.begin();
  //setupTasks();
}

void loop()
{
  gateway.manage.Firebase().operations.updateRequest();
  gateway.manage.Firebase().operations.dataUpload();
}
