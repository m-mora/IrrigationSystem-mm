#include "builder.h"

RelayCollectionBuilder::RelayCollectionBuilder()
{
    relays.clear();
}

RelayCollectionBuilder &RelayCollectionBuilder::setExpander(IOExpander *expander)
{
    this->expander = expander;
    return *this;
}

RelayCollectionBuilder &RelayCollectionBuilder::setTimeProvider(ITimeProvider *expander)
{
    this->timeProvider = timeProvider;
    return *this;
}

RelayCollectionBuilder &RelayCollectionBuilder::setSystemData(SystemData_t *sysData)
{
    this->sysData = sysData;
    return *this;
}

RelayBuilder RelayCollectionBuilder::forPin(IOActionPin_e position)
{
    return RelayBuilder(position, this);
}

LinkedList<IORelay*> RelayCollectionBuilder::build()
{
    return relays;
}

RelayBuilder::RelayBuilder(IOActionPin_e position, RelayCollectionBuilder *parent)
    : parent(parent), position(position), config() {
    if (parent != nullptr) {
        config.timeProvider = parent->timeProvider;
        config.systemData = parent->sysData;
    }
}

RelayBuilder RelayBuilder::forPin(IOActionPin_e position)
{
    parent->relays.add(build());
    return parent->forPin(position);
}

IORelay* RelayBuilder::build()
{
    IORelay* relay = new IORelay(position, config);
    relay->attach_expander(parent->expander);
    return relay;
}

RelayBuilder &RelayBuilder::onTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    config.timeToTurnOn = Time_s (0, 0, 0, hour, minute, second);
    return *this;
}

RelayBuilder &RelayBuilder::onDay(WeekDays_e day)
{
    if (day < DAYS_PER_WEEK) {
        config.WeekDaysToTurnOn.Data |= (1 << (uint8_t)day);
    } else {
        config.WeekDaysToTurnOn.Data = 0b01111111;  // All days...
    }
    return *this;
}

RelayBuilder &RelayBuilder::duration(uint16_t seconds)
{
    config.turnOnDuration = seconds;
    return *this;
}

RelayCollectionBuilder* RelayBuilder::done() {
    return parent;
}