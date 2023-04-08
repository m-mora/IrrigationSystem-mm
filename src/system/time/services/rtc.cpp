#include "rtc.h"
#include "utils/logger.h"

TimeProviderRTC::TimeProviderRTC() {}

/**
 * @brief Initialize the time provider.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderRTC::init()
{
    logger << LOG_INFO << "Initializing RTC..." << EndLine;
    _type = BACKUP;
    if (!rtc.begin())
    {
        logger << LOG_ERROR << "Can't initialize RTC" << EndLine;
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
bool TimeProviderRTC::update()
{
    DateTime now    = rtc.now();
    if (now.unixtime() < BUILD_TIME_UNIX)
    {
        return false;
    }
    datetime.year   = now.year();
    datetime.month  = now.month();
    datetime.day    = now.day();
    datetime.hour   = now.hour();
    datetime.minute = now.minute();
    datetime.second = now.second();
    return true;
}

void TimeProviderRTC::set(Time_s newNow)
{
    rtc.adjust(newNow.toDateTime());
}
