#include "pzem_module.h"

// Constructor initialization list
PZEMModule::PZEMModule()
    : pzemSerial(2),
      pzem(pzemSerial, PZEM_RX_PIN, PZEM_TX_PIN)
{
    voltage = 0;
    current = 0;
    power = 0;
    energy = 0;
    frequency = 0;
    powerFactor = 0;
}

// Initialize UART communication
void PZEMModule::begin()
{
    Serial.println("Initializing PZEM Module...");
}

// Read all parameters from PZEM
void PZEMModule::update()
{
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    energy = pzem.energy();
    frequency = pzem.frequency();
    powerFactor = pzem.pf();

    Serial.printf("V=%.2f I=%.2f P=%.2f E=%.3f F=%.2f PF=%.2f\n",
                  voltage,
                  current,
                  power,
                  energy,
                  frequency,
                  powerFactor);
    // Error handling
    if (isnan(voltage)) voltage = 0;
    if (isnan(current)) current = 0;
    if (isnan(power)) power = 0;
    if (isnan(energy)) energy = 0;
    if (isnan(frequency)) frequency = 0;
    if (isnan(powerFactor)) powerFactor = 0;
}

// Getter functions
float PZEMModule::getVoltage() {return voltage;}

float PZEMModule::getCurrent() {return current;}

float PZEMModule::getPower() {return power;}

float PZEMModule::getEnergy() {return energy;}

float PZEMModule::getFrequency() {return frequency;}

float PZEMModule::getPowerFactor() {return powerFactor;}