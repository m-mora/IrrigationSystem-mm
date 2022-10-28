#include "io_relay.h"
#include "utils/logger.h"

void IORelay::set_config(IORelayConfig_t new_config)
{
    this->config = new_config;
}

void IORelay::update()
{
    if (device == NULL)
    {
        logger << LOG_ERROR << "Device pointer is NULL" << EndLine;
        return;
    }

    if (this->state && isTurnOnDurationTimeOver())
    {
        turnOffRelay();
    }
    else if (!this->state && isMomentToTurnOn())
    {
        turnOnRelay();
    }
}

bool IORelay::isTurnOnDurationTimeOver()
{
    DateTime now = config.timeProvider->get().toDateTime();
    TimeSpan deltaTime = now - storedTurnOnTime;
    logger << LOG_DEBUG << "isTurnOnDurationTimeOver: deltaTime = " << deltaTime.totalseconds() << "s" << EndLine;
    return deltaTime.totalseconds() >= config.turnOnDuration;
}

bool IORelay::isMomentToTurnOn()
{
    bool isTime = false;
    Time_s now = config.timeProvider->get();
    uint8_t dayOfWeek = now.toDateTime().dayOfTheWeek();
    // Check is today on the list of days?
    isTime = (config.WeekDaysToTurnOn.Data & (1 << dayOfWeek)) != 0;
    logger << LOG_DEBUG << "isMomentToTurnOn: isDay? " << isTime << EndLine;

    if (!isTime) {
        return isTime;
    }

    // Check is time?
    Time_s turnOnTime = config.timeToTurnOn;
    turnOnTime.year = now.year;
    turnOnTime.month = now.month;
    turnOnTime.day = now.day;
    TimeSpan deltaTime = now.toDateTime() - turnOnTime.toDateTime();

    isTime = abs(deltaTime.totalseconds()) <= 5;
    logger << LOG_DEBUG << "isMomentToTurnOn: isTime? " << isTime << ", rest seconds: " << deltaTime.totalseconds() << EndLine;

    if (!isTime) {
        return isTime;
    }

    // Check is any other relay on?
    if (config.systemData->Sensors.isAnyValveOn)
    {
        isTime = false;
    }
    logger << LOG_DEBUG << "isMomentToTurnOn: any other relay is on? " << config.systemData->Sensors.isAnyValveOn << EndLine;

    return isTime;
}

void IORelay::turnOffRelay()
{
    logger << LOG_INFO << "Turning off relay " << (uint8_t)position << " on IO addr 0x" << INT_HEX << device->getAddress() << EndLine;
    config.systemData->Sensors.isAnyValveOn = false;
    if (config.voltageRelay) { config.voltageRelay->turn_off(); };
    this->turn_off();
}

void IORelay::turnOnRelay()
{
    logger << LOG_INFO << "Turning on relay " << (uint8_t)position << " on IO addr 0x" << INT_HEX << device->getAddress() << EndLine;
    config.systemData->Sensors.isAnyValveOn = true;
    storedTurnOnTime = config.timeProvider->get().toDateTime();
    if (config.voltageRelay) { config.voltageRelay->turn_on(); };
    this->turn_on();
}
