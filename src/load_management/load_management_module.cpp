#include "load_management_module.h"

String systemStatus = "NORMAL";

LoadManagementModule::LoadManagementModule(
    RelayModule* relayModule,
    float warning,
    float critical,
    float emergency)
{
    relay = relayModule;

    warningThreshold = warning;
    criticalThreshold = critical;
    emergencyThreshold = emergency;
}

void LoadManagementModule::update(float power)
{
    // Normal operation, < 75% load - all relays ON
    if(power < warningThreshold)
    {
        
        systemStatus = "NORMAL";
    }

    // 70-85% load - disconnect lowest priority load
    else if(power >= warningThreshold &&
            power < criticalThreshold)
    {
        relay->relay4(false);
        
        systemStatus = "WARNING";
    }

    // 85-95% load - disconnect medium priority load
    else if(power >= criticalThreshold &&
            power < emergencyThreshold)
    {
        
        relay->relay4(false);
        relay->relay3(false);
        
        systemStatus = "CRITICAL";
    }

    // > 95% load - disconnect highest priority load (Emergency Load shredding)
    else
    {
        
        relay->relay4(false);
        relay->relay3(false);
        relay->relay2(false);
        
        systemStatus = "EMERGENCY";
    }
}

String LoadManagementModule::getStatus()
{
    return systemStatus;
}