#include "irrigation.h"
#include <Arduino.h>

#include "connectivity/wifi.h"
#include "utils/logger.h"
#include "relays/builder.h"

SysLogger logger(nullptr);

#define doUntilTimeElapsed(__time_handler, __ms_elapsed, __function) \
    static uint32_t __time_handler = 0;                              \
    if (__time_handler > millis())                                   \
    {                                                                \
        __time_handler = millis();                                   \
    }                                                                \
    if ((millis() - __time_handler) > __ms_elapsed)                  \
    {                                                                \
        __time_handler = millis();                                   \
        __function                                                   \
    }

IrrigationSystem::IrrigationSystem () {
    timeProviders.clear();
    Status.Sensors.humidityLevel = 0.0f;
    Status.Sensors.isAnyValveOn = false;
    Status.Sensors.isPresenceDetected = false;
    Status.Sensors.isRaining = false;
}

void IrrigationSystem::init()
{
    InitLogger();
    DumpSysInfo();

    InitWifi();
    InitDevices();
    InitSensors();
    InitRelays();

    while (true) {
        Status.sysMilliseconds = millis();

        doUntilTimeElapsed(__update_rtc_handler, 1000, {
            logger << LOG_INFO << "Updating time..." << EndLine;
            timeProviders[0]->update(); // Change of time provider dinamically...
            logger << LOG_INFO << "Updating relay status..." << EndLine;
            _for_each (relays, relay, IORelay*) {
                relay->update();
            }
        });
    }
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

void IrrigationSystem::InitLogger()
{
    Serial.begin(KERNEL_SERIAL_SPEED);
    logger.setLogOutput(&Serial);
    logger.setLogLevel(LOG_DEBUG);
}

void IrrigationSystem::InitWifi()
{
    WifiInitialize();
    TimeProviderNTP *ntpProvider = new TimeProviderNTP();
    if (ntpProvider->init()) {
        timeProviders.add(ntpProvider);
    }
}

void IrrigationSystem::InitDevices()
{
    ioExpander.init(0x00);
    TimeProviderRTC *rtcProvider = new TimeProviderRTC();
    if (rtcProvider->init()) {
        timeProviders.add(rtcProvider);
    }
}

void IrrigationSystem::InitSensors()
{

}

void IrrigationSystem::InitRelays()
{
    // TODO: Create a flexible interface for relay building from eeprom?
    relays = RelayCollectionBuilder::create()
        .setExpander(&ioExpander)
        .setSystemData(&Status)
        .setTimeProvider(timeProviders[0])  // Change of time provider dinamically...
        .forPin(IO_0)
            .onDay(DAYS_PER_WEEK)
            .onTime(23, 0, 0)
            .duration(20)
        .forPin(IO_1)
            .onDay(MONDAY)
            .onDay(WENSDAY)
            .onTime(22, 0, 0)
        .done()
        ->build();
}
