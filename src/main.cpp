#include "config.h"
#include <Arduino.h>
#include "pzem_module.h"
#include "dht_module.h"
#include "relay_module.h"
#include "blynk_module.h"
#include "load_management/load_management_module.h"
#include "logger_module.h"

// Create module objects
PZEMModule pzem;
DHTModule dht;
BlynkModule blynk(WIFI_SSID, WIFI_PASSWORD);
RelayModule relay;

// Assuming a relay priority of 1 > 2 > 3 > 4,
// and thresholds of 840W (warning), 1020W (critical) and 1400W (emergency)
LoadManagementModule loadManager(&relay, 100.0, 200.0, 300.0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 2000;

void setup()
{
    // Initialize serial monitor
    Serial.begin(115200);

    // Initialize modules
    pzem.begin();
    dht.begin();
    blynk.begin();
    relay.begin();
    // Initialize cloud logger
    loggerBegin();

    Serial.println("System Initialization Complete");
}

// Main program loop
void loop()
{
    // Maintain Blynk connection
    blynk.run();

    // Non-blocking timer
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        // Read PZEM sensor data
        pzem.update();

        // Read DHT sensor data
        dht.update();

        // Update load management based on current power
        loadManager.update(pzem.getPower());

        // Send data to Blynk dashboard
        blynk.sendSensorData(
            pzem.getVoltage(),
            pzem.getCurrent(),
            pzem.getPower(),
            pzem.getEnergy(),
            pzem.getFrequency(),
            pzem.getPowerFactor(),
            dht.getTemperature(),
            dht.getHumidity() 
        );

        loggerAddReading(
            pzem.getVoltage(),
            pzem.getCurrent(),
            pzem.getPower(),
            pzem.getEnergy(),
            pzem.getFrequency(),
            pzem.getPowerFactor(),
            dht.getTemperature(),
            dht.getHumidity()
        );
    }

    // Update cloud database with current readings
    loggerUpdate();

}