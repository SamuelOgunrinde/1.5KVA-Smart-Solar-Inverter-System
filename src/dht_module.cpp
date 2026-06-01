#include "dht_module.h"

// Constructor 
DHTModule::DHTModule()
    : dht(DHTPIN, DHTTYPE)
{
    temperature = 0;
    humidity = 0;
}


// Initialize DHT sensor
void DHTModule::begin()
{
    dht.begin();

    Serial.println("DHT22 Module Initialized");
}

// Read sensor data 
void DHTModule::update()
{
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // Handle invalid readings
    if (isnan(temperature))
    {
        temperature = 0;
    }

    if (isnan(humidity))
    {
        humidity = 0;
    }
}

// Get temperature
float DHTModule::getTemperature()
{
    return temperature;
}

// Get humidity
float DHTModule::getHumidity()
{
    return humidity;
}