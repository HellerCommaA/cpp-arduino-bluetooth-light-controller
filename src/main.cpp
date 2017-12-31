#include "Arduino.h"
#include "lights.h"

Lights *gLights = nullptr;

void setup()
{
    gLights = new Lights();
}

void loop()
{
    gLights->run();
}