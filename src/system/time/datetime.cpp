#include "datetime.h"

Time_s::Time_s(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
    :year(year), month(month), day(day), hour(hour), minute(minute), second(second) { }

Time_s::Time_s(DateTime time)
  : year(time.year()), month(time.month()), day(time.day()),
    hour(time.hour()), minute(time.minute()), second(time.second()) { }

String Time_s::toString() const {
  char buff[30];
  toCharArray(buff);
  return String(buff);
}

void Time_s::toCharArray(char *buff) const {
  sprintf(buff, "%02u-%02u-%02uT%02u:%02u:%02uZ", year, month, day, hour, minute, second);
}

DateTime Time_s::toDateTime() const {
  return DateTime(year, month, day, hour, minute, second);
}

bool Time_s::operator==(Time_s& input) {
  return ( this->second == input.second ) && 
         ( this->minute == input.minute ) &&
         ( this->hour == input.hour ) &&
         ( this->day == input.day ) &&
         ( this->month == input.month ) &&
         ( this->year == input.year );
}
