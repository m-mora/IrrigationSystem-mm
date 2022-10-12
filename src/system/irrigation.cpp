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

IrrigationSystem::IrrigationSystem()
{
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

    logger << LOG_INFO << "Initializing System..." << EndLine;
    InitWifi();
    InitDevices();
    InitSensors();
    InitRelays();
    logger << LOG_INFO << "Initialization finished!" << EndLine;

    while (true)
    {
        Status.sysMilliseconds = millis();

        doUntilTimeElapsed(__update_rtc_handler, 1000, {
            timeProviders[0]->update(); // Change of time provider dinamically...
            logger << LOG_DEBUG << (size_t)timeProviders[0] << EndLine;

            logger << LOG_INFO << "Now:  " << timeProviders[0]->get().toString() << EndLine;
            IORelay *relay = NULL;
            for (size_t i = 0; i < relays->size(); i++)
            {
                relay = relays->get(i);
                if (relay == NULL)
                {
                    logger << LOG_WARN << "Found NULL instances while updating relays" << EndLine;
                    continue;
                }
                relay->update();
            }
        });
    }
}

void IrrigationSystem::DumpSysInfo()
{
    logger << LOG_MASTER << EndLine << LOGGER_TEXT_BOLD << LOGGER_TEXT_YELLOW << "--------------------------------------------------------\n"
           << "            _   __ _     _____ _____       \n"
           << "           | | / /| |   |_   _/  __ \\     \n"
           << "           | |/ / | |     | | | /  \\/     \n"
           << "           |    \\ | |     | | | |         \n"
           << "           | |\\  \\| |_____| |_| \\__/\\      \n"
           << "           \\_| \\_/\\_____/\\___/ \\____/     \n"
           << "--------------------------------------------------------\n"
           << LOG_MASTER << "Fimware version: " << KERNEL_VERSION << EndLine << LOG_MASTER << "Build date: " << __DATE__ << " " << __TIME__ << EndLine;
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
    if (ntpProvider->init())
    {
        timeProviders.add(ntpProvider);
    }
}

void IrrigationSystem::InitDevices()
{
    ScanI2CDevicesAndDumpTable();

    ioExpander.init(0x38);

    // TimeProviderRTC *rtcProvider = new TimeProviderRTC();
    // if (rtcProvider->init())
    // {
    //     timeProviders.add(rtcProvider);
    // }
}

void IrrigationSystem::InitSensors()
{
}

void IrrigationSystem::InitRelays()
{
    logger << LOG_INFO << "Creating relay configurations" << EndLine;
    // TODO: Create a flexible interface for relay building from eeprom?
    relays = RelayCollectionBuilder::create()
                 .setExpander(&ioExpander)
                 .setSystemData(&Status)
                 .setTimeProvider(timeProviders[0]) // Change of time provider dinamically...
                 .forPin(IO_0)
                 .onDay(DAYS_PER_WEEK)
                 .onTime(3, 42, 0)
                 .duration(20)
                 .forPin(IO_1)
                 .onDay(DAYS_PER_WEEK)
                 .onTime(3, 43, 0)
                 .duration(30)
                 .done()
                 ->build();
    logger << LOG_INFO << LOGGER_TEXT_GREEN << "Done!" << EndLine;
}

void IrrigationSystem::ScanI2CDevicesAndDumpTable()
{
    uint8_t row = 0;
    char buffer[3];
    Wire.begin();
    I2CDevice test;

    logger << LOG_INFO << F("Scanning I2C Bus ") << EndLine;
    logger << LOG_INFO << F("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F") << EndLine;
    logger << LOG_INFO << F("  00: -- ");

    for (size_t i = 1; i < 0x80; i++)
    {
        test.setAddress(i);

        if (i % 16 == 0)
        {
            row += 0x10;
            sprintf(buffer, "%02X", row);
            logger << EndLine << LOG_INFO << F("  ") << buffer << F(": ");
        }

        if (test.isConnected())
        {
            sprintf(buffer, "%02X", i);
        }
        else
        {
            sprintf(buffer, "--");
        }

        logger << buffer << F(" ");
    }
    logger << EndLine;
}