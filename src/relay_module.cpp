#include "relay_module.h"

// Initialize relay GPIO pins
void RelayModule::begin()
{
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);
    pinMode(RELAY4_PIN, OUTPUT);

    // OFF initially (Active LOW relays)
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    digitalWrite(RELAY3_PIN, HIGH);
    digitalWrite(RELAY4_PIN, HIGH);

    Serial.println("Relay Module Initialized");
}

// Relay ON/OFF control
void RelayModule::relay1(bool state)
{
    digitalWrite(RELAY1_PIN, !state);
}

void RelayModule::relay2(bool state)
{
    digitalWrite(RELAY2_PIN, !state);
}

void RelayModule::relay3(bool state)
{
    digitalWrite(RELAY3_PIN, !state);
}

void RelayModule::relay4(bool state)
{
    digitalWrite(RELAY4_PIN, !state);
}