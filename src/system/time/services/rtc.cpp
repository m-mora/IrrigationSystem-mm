#include "rtc.h"

/**
 * @brief Initialize the time provider.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderRTC::init() {
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
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
bool TimeProviderRTC::update() {
    this->datetime = { 0, 0, 0, 0, 0, 0 };
    DateTime now = rtc.now();
    datetime.year   = now.year();
    datetime.month  = now.month();
    datetime.day    = now.day();
    datetime.minute = now.minute();
    datetime.second = now.second();
    return true;
}