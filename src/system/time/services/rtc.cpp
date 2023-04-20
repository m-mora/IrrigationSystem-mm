#include "rtc.h"

#include "utils/logger.h"

TimeProviderRTC::TimeProviderRTC() {}

/**
 * @brief Initialize the time provider.
 *
 * @return true  On success
 * @return false On failure
 */
bool TimeProviderRTC::init() {
  logger << LOG_INFO << "Initializing RTC..." << EndLine;
  _type = BACKUP;
  _secondsThreshold = 1;
  if (!rtc.begin()) {
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
bool TimeProviderRTC::update() {
  DateTime now = rtc.now();
  if (now.unixtime() <
      BUILD_TIME_UNIX -
          21600)  // TODO: fix the compiler time to considere time zone offset
  {
    return false;
  }
  datetime.year = now.year();
  datetime.month = now.month();
  datetime.day = now.day();
  datetime.hour = now.hour();
  datetime.minute = now.minute();
  datetime.second = now.second();
  return true;
}

void TimeProviderRTC::set(Time_s newNow) { rtc.adjust(newNow.toDateTime()); }
