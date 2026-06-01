#include "config.h"
#include <Arduino.h>

#include "pzem_module.h"
#include "relay_module.h"
#include "dht_module.h"
#include "blynk_module.h"

// Create module objects
PZEMModule pzem;
DHTModule dht;
RelayModule relay;
BlynkModule blynk(WIFI_SSID, WIFI_PASSWORD);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 2000;

void setup()
{
    // Initialize serial monitor
    Serial.begin(115200);

    Serial.println();
    Serial.println("SMART INVERTER MONITORING SYSTEM");

    // Initialize modules
    relay.begin();
    blynk.begin();
    pzem.begin();
    dht.begin();

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

        // Print values to serial monitor
        Serial.println("========== SENSOR DATA ==========");

        Serial.print("Voltage: ");
        Serial.print(pzem.getVoltage());
        Serial.println(" V");

        Serial.print("Current: ");
        Serial.print(pzem.getCurrent());
        Serial.println(" A");

        Serial.print("Power: ");
        Serial.print(pzem.getPower());
        Serial.println(" W");

        Serial.print("Energy: ");
        Serial.print(pzem.getEnergy());
        Serial.println(" kWh");

        Serial.print("Frequency: ");
        Serial.print(pzem.getFrequency());
        Serial.println(" Hz");

        Serial.print("Power Factor: ");
        Serial.println(pzem.getPowerFactor());

        Serial.print("Temperature: ");
        Serial.print(dht.getTemperature());
        Serial.println(" °C");

        Serial.print("Humidity: ");
        Serial.print(dht.getHumidity());
        Serial.println(" %");

        Serial.println("================================");

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
    }
}