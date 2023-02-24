/*----------------------------------------------------------------------*
 *  _   __ _     _____ _____                                            *
 * | | / /| |   |_   _/  __ \   This software is been developed by      *
 * | |/ / | |     | | | /  \/   a group of enthusiast hobbiest          *
 * |    \ | |     | | | |       with the purpose of learn and           *
 * | |\  \| |_____| |_| \__/\   have fun, so nobody is responsible or   *
 * \_| \_/\_____/\___/ \____/   will provide warranty.                  *
 *                                                                      *
 * This software will run in a ESP8266 microcontrolller, the objective  *
 * is to have a irrigation system controler that can read some sensors  *
 * and decide if a valve should be open.                                *
 * There is not restriction to use, modify and improve the code, so     *
 * please do it and share the improvements.                             *
 *                                                                      *
 * Letś have Fun!!                                                      *
 *                                                                      *
 * ---------------------------------------------------------------------*/
#if !defined(__IRRIGATION_SYSTEM_TIME_H__)
#define __IRRIGATION_SYSTEM_TIME_H__

#include <stdint.h>
#include <Arduino.h>
#include <RTClib.h>

typedef enum {
  SUNDAY = (1 << 0),
  MONDAY = (1 << 1),
  TUESDAY = (1 << 2),
  WENSDAY = (1 << 3),
  THURSDAY = (1 << 4),
  FRIDAY = (1 << 5),
  SATURNDAY = (1 << 6),
  ALL_WEEK = 0b01111111
} WeekDays_e;

struct CompilationTime_t {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

struct Time_s {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  
  Time_s() { }
  Time_s(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
  Time_s(DateTime time);
  Time_s(CompilationTime_t c_time);

  String toString() const;
  void toCharArray(char *buff) const;
  DateTime toDateTime() const;
  bool operator==(Time_s&);
};
typedef struct Time_s Time_s;

#define __SECONDS_FROM_1970_TO_2000                                              \
  946684800 ///< Unixtime for 2000-01-01 00:00:00, useful for initialization

constexpr uint16_t __conv2d(const char *p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

constexpr uint16_t __date2days(uint16_t y, uint8_t m, uint8_t d) {
  const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30};
  if (y >= 2000U)
    y -= 2000U;
  uint16_t days = d;
  for (uint8_t i = 1; i < m; ++i)
    days += daysInMonth[i - 1];
  if (m > 2 && y % 4 == 0)
    ++days;
  return days + 365 * y + (y + 3) / 4 - 1;
}

constexpr uint32_t __time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
  return ((days * 24UL + h) * 60 + m) * 60 + s;
}

constexpr CompilationTime_t getBuildTime(const char *date, const char *time)
{
    CompilationTime_t dt{};
    dt.year = __conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0])
    {
    case 'J': dt.month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
    case 'F': dt.month = 2; break;
    case 'A': dt.month = date[2] == 'r' ? 4 : 8; break;
    case 'M': dt.month = date[2] == 'r' ? 3 : 5; break;
    case 'S': dt.month = 9; break;
    case 'O': dt.month = 10; break;
    case 'N': dt.month = 11; break;
    case 'D': dt.month = 12; break;
    }
    dt.day = __conv2d(date + 4);
    dt.hour = __conv2d(time);
    dt.minute = __conv2d(time + 3);
    dt.second = __conv2d(time + 6);

    return dt;
}

constexpr uint32_t getBuildTimeAsUnixTime(const char *date, const char *time)
{
    CompilationTime_t dt = getBuildTime (date, time);
    uint32_t unixTime = 0;
    unixTime = __time2ulong(__date2days(dt.year, dt.month, dt.day), dt.hour, dt.month, dt.minute) + __SECONDS_FROM_1970_TO_2000;
    return unixTime;
}

#endif // __IRRIGATION_SYSTEM_TIME_H__
