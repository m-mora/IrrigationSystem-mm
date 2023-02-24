#include "builder.h"
#include "utils/logger.h"

RelayCollectionBuilder::RelayCollectionBuilder()
{
    relays = new LinkedList<WaterValve*>();
    relays->clear();
}

RelayCollectionBuilder &RelayCollectionBuilder::setExpander(IOExpander *expander)
{
    this->expander = expander;
    return *this;
}

RelayCollectionBuilder &RelayCollectionBuilder::setTimeProvider(ITimeProvider *provider)
{
    this->timeProvider = provider;
    return *this;
}

RelayCollectionBuilder &RelayCollectionBuilder::setSystemData(SystemData_t *sysData)
{
    this->sysData = sysData;
    return *this;
}

RelayCollectionBuilder &RelayCollectionBuilder::setVoltageValvePin(IOActionPin_e pin) 
{
    this->voltageValvePin = pin;
    return *this;
}

RelayBuilder RelayCollectionBuilder::forPin(IOActionPin_e position)
{
    logger << LOG_DEBUG << "Build new relay for bit " << (uint8_t)position << EndLine;
    return RelayBuilder(position, this);
}

LinkedList<WaterValve*> *RelayCollectionBuilder::build()
{
    logger << LOG_DEBUG << "Relay created " << relays->size() << EndLine;
    return relays;
}

RelayBuilder::RelayBuilder(IOActionPin_e position, RelayCollectionBuilder *parent)
    : parent(parent), position(position), config() {
    if (parent != nullptr) {
        config.timeProvider = parent->timeProvider;
        config.systemData = parent->sysData;
        config.voltageRelay = new IOAction(parent->voltageValvePin);
        config.voltageRelay->attach_expander(parent->expander);
    }
}

RelayBuilder RelayBuilder::forPin(IOActionPin_e position)
{
    if (parent == NULL) {
        logger << LOG_ERROR << "Trying to build relay without parent collection" << EndLine;
    }
    parent->relays->add(build());
    return parent->forPin(position);
}

WaterValve* RelayBuilder::build()
{
    WaterValve* relay = new WaterValve(position, config);
    relay->attach_expander(parent->expander);
    return relay;
}

RelayBuilder &RelayBuilder::onTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    config.timeToTurnOn = Time_s (0, 0, 0, hour, minute, second);
    return *this;
}

RelayBuilder &RelayBuilder::onDays(WeekDays_e days)
{
    config.WeekDaysToTurnOn.Data = days;
    return *this;
}

RelayBuilder &RelayBuilder::duration(uint16_t seconds)
{
    config.turnOnDuration = seconds;
    return *this;
}

RelayCollectionBuilder* RelayBuilder::done() {
    logger << LOG_DEBUG << "Finishing to create relays, parent address = 0x" << INT_HEX << (size_t)parent << EndLine;
    parent->relays->add(build());
    return parent;
}