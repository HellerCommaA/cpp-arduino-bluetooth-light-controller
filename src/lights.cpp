
#include <Thread.h>
#include <stdarg.h>
#include "lights.h"
#include "Arduino.h"
#include "pins.h"

void ttyprintf(char *fmt, ... ){
    char buf[128];
    va_list args;
    va_start (args, fmt );
    vsnprintf(buf, 128, fmt, args);
    va_end (args);
    Serial.print(buf);
}

String Lights::serialValue = "";

Lights::Lights()
{
    // begins comms over bt serial
    Serial.begin(9600);
    delay(200);

    pinMode(BUILTIN, OUTPUT);
    pinMode(LED, OUTPUT);
    ttyprintf("connection established\n");
    serialThread = new Thread();
    updateLightsThread = new Thread();
}

void
Lights::serialRead()
{
    if (Serial.available()) {
        // reads from bt module
        Lights::serialValue = Serial.readString();
        ttyprintf("got command: %s\n", serialValue.c_str());
    }
}

void
Lights::updateLights()
{
    if(serialValue == "1") {
        digitalWrite(BUILTIN, HIGH);
        digitalWrite(LED, LOW);
        ttyprintf("turning LED on\n");
        serialValue = "x";
    }
    if (serialValue == "0") {
        digitalWrite(BUILTIN, LOW);
        digitalWrite(LED, HIGH);
        ttyprintf("turning LED off\n");
        serialValue = "x";
    }
}

void
Lights::run()
{
    serialThread->onRun(Lights::serialRead);
    serialThread->setInterval(50);

    updateLightsThread->onRun(Lights::updateLights);
    updateLightsThread->setInterval(50);

    if (serialThread->shouldRun())
        serialThread->run();
    if (updateLightsThread->shouldRun())
        updateLightsThread->run();
}