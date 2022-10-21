#include "rtc.h"
#include "utils/logger.h"

#define MIN_UNIX_TIME getBuildTimeAsUnixTime (__DATE__, __TIME__)

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
    if (!rtc.begin())
    {
        logger << LOG_ERROR << "Can't initialize RTC" << EndLine;
        return false;
    }

    DateTime BuildTime(F(__DATE__), F(__TIME__));
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
    this->datetime = {0, 0, 0, 0, 0, 0};
    DateTime now = rtc.now();
    datetime.year = now.year();
    datetime.month = now.month();
    datetime.day = now.day();
    datetime.minute = now.minute();
    datetime.second = now.second();
    if (now.unixtime() < MIN_UNIX_TIME)
    {
        return false;
    }
    return true;
}

void TimeProviderRTC::set(Time_s newNow)
{
    rtc.adjust(newNow.toDateTime());
}
