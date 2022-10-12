#include "ntp.h"
#include "utils/time_converter.h"
#include "utils/logger.h"
#include "../../connectivity/wifi.h"

// Define NTP properties
#define NTP_OFFSET   -5 * 60 * 60    // utc offset
#define NTP_INTERVAL 60 * 1000       // miliseconds
#define NTP_ADDRESS  "pool.ntp.org"  // URL NTP Server

#define NTP_ATTMPS  5
#define UTC_DATE    1665179521  // 1665179521 =  Friday, 7 October 2022 12:00:00 AM

TimeProviderNTP::TimeProviderNTP() : timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL) {}

/**
 * @brief Initialize the time provider.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::init()
{
    logger << LOG_INFO << "Initializing NTP..." << EndLine;
    timeClient.begin();

    return this->updateNTP();
}

/**
 * @brief Update the internal datetime.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::update()
{
    this->datetime = {0, 0, 0, 0, 0, 0};
    
    if(!this->updateNTP())
    {
        return false;
    }
    DateTime _dateTime = DateTime(timeClient.getEpochTime());
    datetime.year = _dateTime.year();
    datetime.month = _dateTime.month();
    datetime.day = _dateTime.day();
    datetime.hour = _dateTime.hour();
    datetime.minute = _dateTime.minute();
    datetime.second = _dateTime.second();

    return true;
}

bool TimeProviderNTP::updateUTCTime()
{
    bool success = timeClient.forceUpdate();
    if (!success) {
        return false;
    }

    epochUtcTime = timeClient.getEpochTime();
    return epochUtcTime >= UTC_DATE;
}

bool TimeProviderNTP::updateNTP()
{
    bool success = false;
    if (WiFi.status() != WL_CONNECTED) 
    {
        logger << LOG_WARN << "Wifi disconnected " << EndLine;
        return false;
    }

    for(uint8 tries = 0; (tries < NTP_ATTMPS) && (!success); tries++)
    {
        success = updateUTCTime();
    }

    if(!success) {
        logger << LOG_INFO << "Connection to NTP Server " << LOGGER_TEXT_RED << "FAILED" << EndLine;
        return false;
    }

    return true;
}
