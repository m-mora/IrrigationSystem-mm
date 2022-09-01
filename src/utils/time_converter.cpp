#include "time_converter.h"

#define YEAR 31556926
#define MONTH 2629743
#define WEEK 604800
#define DAY 86400
#define HOUR 3600
#define MINUTE 60

int year_zero = 1970;

datetime_t TimeConverter(unsigned long epoch){
    datetime_t datetime;
    int residue = 0;
    //Year
    datetime.year = year_zero+((int)(epoch/YEAR));
    residue = epoch-(YEAR*datetime.year);
    //Month
    datetime.month = (int)(residue/MONTH);
    residue = residue-(MONTH*datetime.month);
    //Day
    datetime.day = (int)(residue/DAY);
    residue = residue-(DAY*datetime.day);
    //Hour
    datetime.hour = (int)(residue/HOUR);
    residue = residue-(HOUR*datetime.hour);
    //Minute
    datetime.minute = (int)(residue/MINUTE);
    residue = residue-(MINUTE*datetime.minute);
    //Second
    datetime.second = residue;
    return datetime;
};