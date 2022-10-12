#include "ntp.h"
#include "utils/time_converter.h"
#include "utils/logger.h"
#include "../../connectivity/wifi.h"

// Define NTP properties
#define NTP_OFFSET   60 * 60         // seconds
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
    
    if(this->updateNTP())
    {
        return false;
    }

    datetime = getDateTimeFromEpochTime(timeClient.getEpochTime());
    logger << LOG_DEBUG << "Get Time " << datetime.toString() << EndLine;

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
    logger << LOG_DEBUG << "Conection status: " << WiFi.status() << EndLine;

    for(uint8 tries = 0; (tries < NTP_ATTMPS) && (!success); tries++)
    {
        success = updateUTCTime();
    }

    if(!success) {
        logger << LOG_INFO << "Connection to NTP Server " << LOGGER_TEXT_RED << "FAILED" << EndLine;
        return false;
    }

    logger << LOG_INFO << "Connection to NTP Server " << LOGGER_TEXT_GREEN << "SUCCESSFUL" << EndLine;
    logger << LOG_DEBUG << "UTP read " << epochUtcTime << " Compared to: " << UTC_DATE << EndLine;
    return true;
}
