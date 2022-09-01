#include "ntp.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "utils/time_converter.cpp"


#define NTP_OFFSET   60 * 60         // seconds
#define NTP_INTERVAL 60 * 1000       // miliseconds
#define NTP_ADDRESS  "pool.ntp.org"  // URL NTP Server

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

/**
 * @brief Initialize the time provider.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::init() {
    timeClient.begin();
    return true;
}

/**
 * @brief Update the internal datetime.
 * 
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::update() {
    timeClient.update();
    this->datetime = { 0, 0, 0, 0, 0, 0 };
    datetime = TimeConverter(timeClient.getEpochTime());
    return true;
}