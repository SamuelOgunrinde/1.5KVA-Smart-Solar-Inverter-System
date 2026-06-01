#ifndef DHT_MODULE_H
#define DHT_MODULE_H

#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

class DHTModule
{
private:
    DHT dht;

    float temperature;
    float humidity;
    
public:
    // Constructor
    DHTModule();

    // Initialize DHT sensor
    void begin();

    // Update sensor readings
    void update();

    // Getter functions
    float getTemperature();

    float getHumidity();
};

#endif