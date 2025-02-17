#include "ntp.h"

#include <stdint.h>

#include "../../connectivity/wifi.h"
#include "utils/logger.h"
#include "utils/time_converter.h"

#define MINUTES_TO_SECONDS(__minutes) (__minutes * 60)

// Define NTP properties
#define NTP_OFFSET -6 * 60 * 60     // utc offset
#define NTP_INTERVAL 60 * 1000      // miliseconds
#define NTP_ADDRESS "pool.ntp.org"  // URL NTP Server
#define NTP_ATTMPS 5

TimeProviderNTP::TimeProviderNTP()
    : timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL) {}

/**
 * @brief Initialize the time provider.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::init() {
  logger << LOG_INFO << "Initializing NTP..." << EndLine;
  timeClient.begin();
  _type = PRIMARY;
  _secondsThreshold = MINUTES_TO_SECONDS(10);

  return this->updateNTP();
}

/**
 * @brief Update the internal datetime.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderNTP::update() {
  bool status = this->updateNTP();

  DateTime _dateTime = DateTime(timeClient.getEpochTime());
  logger << LOG_DEBUG << " Updated NTP date: " << Time_s(_dateTime).toString() << EndLine;

  if (!status || (_dateTime.year() == 2036)) {
    return false;
  }
  datetime.year = _dateTime.year();
  datetime.month = _dateTime.month();
  datetime.day = _dateTime.day();
  datetime.hour = _dateTime.hour();
  datetime.minute = _dateTime.minute();
  datetime.second = _dateTime.second();

  return true;
}

bool TimeProviderNTP::updateUTCTime() {
  bool success = timeClient.forceUpdate();
  if (!success) {
    return false;
  }

  epochUtcTime = timeClient.getEpochTime();
  return epochUtcTime >=
         BUILD_TIME_UNIX - 21600;  // TODO: fix the compiler time to considere
                                   // time zone offset
}

bool TimeProviderNTP::updateNTP() {
  bool success = false;
  if (WiFi.status() != WL_CONNECTED) {
    logger << LOG_WARN << "Wifi disconnected " << EndLine;
    return false;
  }

  for (uint8_t attempts = 0; (attempts < NTP_ATTMPS) && (!success);
       attempts++) {
    success = updateUTCTime();
  }

  if (!success) {
    logger << LOG_INFO << "Connection to NTP Server " << LOGGER_TEXT_RED
           << "FAILED" << EndLine;
    return false;
  }

  return true;
}
