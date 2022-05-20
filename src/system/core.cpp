#include "core.h"

#include "connectivity/wifi.h"


IrrigationSystem::IrrigationSystem () {
}

void IrrigationSystem::init() {
    WifiInitialize();
}
