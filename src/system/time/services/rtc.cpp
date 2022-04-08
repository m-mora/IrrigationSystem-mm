#include "rtc.h"

/**
 * @brief Initialize the time provider.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderRTC::init() {
    this->datetime = { 0, 0, 0, 0, 0, 0 };
    return true;
}

/**
 * @brief Update the internal datetime.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderRTC::update() {
    return true;
}

/**
 * @brief Returns a copy of the internal datetime.
 * 
 * @return datetime_t Last updated datetime.
 */
datetime_t TimeProviderRTC::get() {
    return this->datetime;
}