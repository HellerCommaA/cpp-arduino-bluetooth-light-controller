#include "lights.h"
#include "Arduino.h"
#include "pins.h"
#include <Thread.h>
#include <stdarg.h>
#include "SoftwareSerial.h"

void ttyprintf(char *fmt, ... ){
    char buf[128];
    va_list args;
    va_start (args, fmt );
    vsnprintf(buf, 128, fmt, args);
    va_end (args);
    Serial.print(buf);
}

String Lights::serialValue = "";
SoftwareSerial gSerial(0, 1);

Lights::Lights()
{
    gSerial.begin(9600);

    pinMode(BUILTIN, HIGH);
    Serial.begin(9600);
    
    serialThread = new Thread();
    updateLightsThread = new Thread();
}

void
Lights::serialRead()
{
    if(gSerial.available()) {
        Lights::serialValue = gSerial.readString();
        ttyprintf("got: '%s'\n", serialValue.c_str());
    }
}

void
Lights::updateLights()
{

    if(serialValue == "1") {
        digitalWrite(BUILTIN, HIGH);
    }
    if (serialValue == "2") {
        digitalWrite(BUILTIN, LOW);
    }
}

void
Lights::run()
{
    serialThread->onRun(Lights::serialRead);
    serialThread->setInterval(500);

    updateLightsThread->onRun(Lights::updateLights);
    updateLightsThread->setInterval(500);
    
    if (serialThread->shouldRun())
        serialThread->run();

    if (updateLightsThread->shouldRun())
        updateLightsThread->run();
}