#include "config.h"
#include "blynk_module.h"
#include "relay_module.h"
#include "load_management/load_management_module.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>


// Constructor
BlynkModule::BlynkModule(const char* wifiSSID,
                         const char* wifiPassword)
{
    ssid = wifiSSID;
    password = wifiPassword;
}

extern RelayModule relay;
// extern LoadManagementModule loadManager;

// Initialize WiFi and Blynk
void BlynkModule::begin()
{
    Serial.println("Initializing Blynk...");

    // Connect to WiFi + Blynk Cloud
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

    Serial.println("Blynk Connected");
}

// Maintain cloud communication
void BlynkModule::run()
{
    Blynk.run();
}

// Check Blynk connection status
bool BlynkModule::isConnected()
{
    return Blynk.connected();
}

// Send PZEM parameters to Blynk dashboard
void BlynkModule::sendSensorData(float voltage,
                               float current,
                               float power,
                               float energy,
                               float frequency,
                               float powerFactor,
                               float temperature,
                               float humidity)
{
    // Virtual Pin Mapping
    Blynk.virtualWrite(V0, voltage);
    Blynk.virtualWrite(V1, current);
    Blynk.virtualWrite(V2, power);
    Blynk.virtualWrite(V3, energy);
    Blynk.virtualWrite(V4, frequency);
    Blynk.virtualWrite(V5, powerFactor);
    Blynk.virtualWrite(V6, temperature);
    Blynk.virtualWrite(V7, humidity);
    // Blynk.virtualWrite(V20, loadManager.getStatus());  
}

// Relay 1
BLYNK_WRITE(V10)
{
    relay.relay1(param.asInt());
}

// Relay 2
BLYNK_WRITE(V11)
{
    relay.relay2(param.asInt());
}

// Relay 3
BLYNK_WRITE(V12)
{
    relay.relay3(param.asInt());
}

// Relay 4
BLYNK_WRITE(V13)
{
    relay.relay4(param.asInt());
}



