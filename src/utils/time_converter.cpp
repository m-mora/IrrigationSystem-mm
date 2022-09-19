#include "time_converter.h"

#define SECONDS_IN_YEAR 31556926
#define SECONDS_IN_MONTH 2629743
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
#define YEAR_ZERO 1970

datetime_t getDateTimeFromEpochTime(unsigned long epoch){
    datetime_t datetime;
    int residue = 0;
    //Year
    datetime.year = (uint8_t) YEAR_ZERO+(epoch/SECONDS_IN_YEAR);
    residue = epoch-(SECONDS_IN_YEAR*datetime.year);
    //Month
    datetime.month = (uint8_t)(residue/SECONDS_IN_MONTH);
    residue = residue-(SECONDS_IN_MONTH*datetime.month);
    //Day
    datetime.day = (uint8_t)(residue/SECONDS_IN_DAY);
    residue = residue-(SECONDS_IN_DAY*datetime.day);
    //Hour
    datetime.hour = (uint8_t)(residue/SECONDS_IN_HOUR);
    residue = residue-(SECONDS_IN_HOUR*datetime.hour);
    //Minute
    datetime.minute = (uint8_t)(residue/SECONDS_IN_MINUTE);
    residue = residue-(SECONDS_IN_MINUTE*datetime.minute);
    //Second
    datetime.second = residue;
    return datetime;
};