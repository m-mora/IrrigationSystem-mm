#include "core.h"
#include <Arduino.h>

#include "connectivity/wifi.h"
#include "utils/logger.h"

SysLogger logger(nullptr);

IrrigationSystem::IrrigationSystem () { }

void IrrigationSystem::init() {
    // Initialize serial to initializa the logger.
    Serial.begin(115200);
    logger.setLogOutput(&Serial);
    logger.setLogLevel(LOG_DEBUG);

    // Dump firmware information
    DumpSysInfo();

    logger << LOG_MASTER << LOGGER_TEXT_YELLOW << "Initializing system" << EndLine;
    // Setup WiFi interface
    WifiInitialize();
}

void IrrigationSystem::DumpSysInfo() {
    logger << LOG_MASTER << EndLine << LOGGER_TEXT_BOLD << LOGGER_TEXT_YELLOW <<
        "--------------------------------------------------------\n" <<
        "            _   __ _     _____ _____ "      <<
        "           | | / /| |   |_   _/  __ \\"     <<
        "           | |/ / | |     | | | /  \\/"     <<
        "           |    \\ | |     | | | |    "     <<
        "           | |\\  \\| |_____| |_| \\__/\\"  <<
        "           \\_| \\_/\\_____/\\___/ \\____/" <<
        "--------------------------------------------------------\n" <<
        LOG_MASTER << "Fimware version: " << KERNEL_VERSION << EndLine <<
        LOG_MASTER << "Build date: " << __DATE__ << " " << __TIME__ << EndLine;
}
