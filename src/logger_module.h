#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#include <Arduino.h>

void loggerBegin();
void loggerUpdate();

// Call this once per PZEM/DHT/relay read cycle to feed
// the 5-minute accumulator (replaces the old UART ingestion)
void loggerAddReading(
    float voltage,
    float current,
    float power,
    float energy,
    float frequency,
    float powerFactor,
    float temperature,
    float humidity
);

#endif