#include "ntp.h"
#include "utils/time_converter.h"
#include "utils/logger.h"
#include "../../connectivity/wifi.h"
// Define NTP properties
#define NTP_OFFSET   60 * 60         // seconds
#define NTP_INTERVAL 60 * 1000       // miliseconds
#define NTP_ADDRESS  "pool.ntp.org"  // URL NTP Server

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
    
    return this->UpdateNTP();
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
    
    if(this->UpdateNTP())
    {
        return false;
    }
    datetime = getDateTimeFromEpochTime(timeClient.getEpochTime());
    return true;
}

bool TimeProviderNTP::UpdateNTP()
{
    uint16 utc;
    logger << LOG_DEBUG << "Conection status: " << WiFi.status() << EndLine;
    timeClient.forceUpdate();
    timeClient.update();
    utc = timeClient.getEpochTime();
    logger << LOG_DEBUG << "UTP read " << utc << " Compared to: " << UTC_DATE << EndLine;
    logger << LOG_DEBUG << "Get Time " << timeClient.getFormattedTime() << EndLine;
    
    for(uint8 tries = 0; (tries < NTP_ATTMPS) && (utc < UTC_DATE); tries++)
    {
        timeClient.update();
        utc = timeClient.getEpochTime();
    }
    if(utc < UTC_DATE) {
        logger << LOG_INFO << "Connection to NTP Server FAILED" << EndLine;
        return false;
    }
    logger << LOG_INFO << "Connection to NTP Server SUCCESSFUL" << EndLine;
    return true;
}
