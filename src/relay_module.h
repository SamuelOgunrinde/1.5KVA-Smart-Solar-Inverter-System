#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include <Arduino.h>

// Relay GPIO pins
#define RELAY1_PIN 25
#define RELAY2_PIN 26
#define RELAY3_PIN 27
#define RELAY4_PIN 14

class RelayModule
{
public:
    void begin();

    // Relay control functions
    void relay1(bool state);
    void relay2(bool state);
    void relay3(bool state);
    void relay4(bool state);
};

#endif