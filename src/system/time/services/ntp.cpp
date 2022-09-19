#include "ntp.h"
#include "utils/time_converter.h"

TimeProviderNTP::TimeProviderNTP() : timeClient(ntpUDP) {}

/**
 * @brief Initialize the time provider.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::init()
{
    timeClient.begin();
    timeClient.update();
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
    timeClient.update();
    datetime = getDateTimeFromEpochTime(timeClient.getEpochTime());
    return true;
}