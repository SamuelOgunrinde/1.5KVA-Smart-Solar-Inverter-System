#ifndef PZEM_MODULE_H
#define PZEM_MODULE_H

#include <Arduino.h>
#include <PZEM004Tv30.h>

// UART2 pins for ESP32
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

class PZEMModule
{
private:
    HardwareSerial pzemSerial;
    PZEM004Tv30 pzem;

    // Measured parameters
    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float powerFactor;

public:
    // Constructor
    PZEMModule();

    // Initialize PZEM communication
    void begin();

    // Read all electrical parameters
    void update();

    // Getter functions
    float getVoltage();
    float getCurrent();
    float getPower();
    float getEnergy();
    float getFrequency();
    float getPowerFactor();
};

#endif