#include "ntp.h"
#include "utils/time_converter.h"
#include "utils/logger.h"

TimeProviderNTP::TimeProviderNTP() : timeClient(ntpUDP) {}

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
    if(this->UpdateNtp())
    {
        return false;
    }
    return true;
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
    
    if(this->UpdateNtp())
    {
        return false;
    }
    datetime = getDateTimeFromEpochTime(timeClient.getEpochTime());
    return true;
}

bool TimeProviderNTP::UpdateNtp()
{
    uint16 utc;
    timeClient.forceUpdate();
    timeClient.update();
    utc = timeClient.getEpochTime();
    // 1665179521 =  Friday, 7 October 2022 12:00:00 AM
    for(uint8 tries = 0; (tries < 5) && (utc < 1665179521); tries++)
    {
        timeClient.update();
    }
    if(utc < 1665179521) {
        return false;
    }
    return true;
}
