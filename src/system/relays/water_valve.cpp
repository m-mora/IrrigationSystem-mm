#include "water_valve.h"
#include "utils/logger.h"

void WaterValve::set_config(IORelayConfig_t new_config)
{
    this->config = new_config;
}

void WaterValve::update()
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

bool WaterValve::isTurnOnDurationTimeOver()
{
    DateTime now = config.timeProvider->get().toDateTime();
    TimeSpan deltaTime = now - storedTurnOnTime;
    logger << LOG_DEBUG << "isTurnOnDurationTimeOver: deltaTime = " << deltaTime.totalseconds() << "s" << EndLine;
    return deltaTime.totalseconds() >= config.turnOnDuration;
}

bool WaterValve::isMomentToTurnOn()
{
    bool isTime = false;
    Time_s now = config.timeProvider->get();
    uint8_t dayOfWeek = now.toDateTime().dayOfTheWeek();
    // Check is today on the list of days?
    isTime = (config.WeekDaysToTurnOn.Data & (1 << dayOfWeek)) != 0;
    logger << LOG_DEBUG << "isMomentToTurnOn: isDay? " << (isTime ? "Yes": "No") << EndLine;
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
    logger << LOG_DEBUG << "isMomentToTurnOn: isTime? " << isTime << ", remaining time: " << -deltaTime.hours() << ":"  << -deltaTime.minutes() << ":" << -deltaTime.seconds() << EndLine;

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

void WaterValve::turnOffRelay()
{
    config.systemData->Sensors.isAnyValveOn = false;
    this->turn_off();
    logger << LOG_INFO << "Turning off relay " << (uint8_t)position << " on IO addr 0x" << INT_HEX << device->getAddress() << EndLine;
    if (config.voltageRelay) {
        logger << LOG_DEBUG << " Calling Voltage Relay in pos " << config.voltageRelay->getPosition() << EndLine;
        config.voltageRelay->turn_off();
    };
}

void WaterValve::turnOnRelay()
{
    config.systemData->Sensors.isAnyValveOn = true;
    storedTurnOnTime = config.timeProvider->get().toDateTime();
    if (config.voltageRelay) {
        logger << LOG_DEBUG << " Calling Voltage Relay in pos " << config.voltageRelay->getPosition() << EndLine;
        config.voltageRelay->turn_on();
    };
    logger << LOG_INFO << "Turning on relay " << (uint8_t)position << " on IO addr 0x" << INT_HEX << device->getAddress() << EndLine;
    this->turn_on();
}
