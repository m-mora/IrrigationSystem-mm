#include "ntp.h"

#define NTP_OFFSET   60 * 60         // seconds
#define NTP_INTERVAL 60 * 1000       // miliseconds
#define NTP_ADDRESS  "pool.ntp.org"  // URL NTP Server

/**
 * @brief Initialize the time provider.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::init() {
    this->datetime = { 0, 0, 0, 0, 0, 0 };
    return true;
}

/**
 * @brief Update the internal datetime.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::update() {
    return true;
}