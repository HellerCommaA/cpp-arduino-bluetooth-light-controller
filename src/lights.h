#ifndef LIGHTS_HEADER
#define LIGHTS_HEADER
#include <Thread.h>
#include <SoftwareSerial.h>
#include "Arduino.h"

class Lights
{
public:
    Lights();
    ~Lights() {};  
    void run();

protected:
    static void serialRead();
    static void updateLights();
    Thread *serialThread = nullptr;
    Thread *updateLightsThread = nullptr;
    static String serialValue;
};




#endif