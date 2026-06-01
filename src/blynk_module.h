#ifndef BLYNK_MODULE_H 
#define BLYNK_MODULE_H

#include <Arduino.h>

class BlynkModule
{
private:
    const char* ssid;
    const char* password;

public:
    // Constructor
    BlynkModule(const char* wifiSSID,
                const char* wifiPassword);

    // Initialize Blynk + WiFi
    void begin();

    // Maintain Blynk connection
    void run();

    // Send sensor data to dashboard
    void sendSensorData(float voltage,
                    float current,
                    float power,
                    float energy,
                    float frequency,
                    float powerFactor,
                    float temperature,
                    float humidity);

    // Connection status
    bool isConnected();
};

#endif