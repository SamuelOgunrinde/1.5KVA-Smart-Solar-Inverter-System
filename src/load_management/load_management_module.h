#ifndef LOAD_MANAGEMENT_MODULE_H
#define LOAD_MANAGEMENT_MODULE_H

#include <Arduino.h>
#include "relay_module.h"

class LoadManagementModule
{
private:
    RelayModule* relay;

    float warningThreshold;
    float criticalThreshold;
    float emergencyThreshold;

public:
    LoadManagementModule(
        RelayModule* relayModule,
        float warning,
        float critical,
        float emergency);

    void update(float power);

    String getStatus();
};

#endif