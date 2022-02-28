/************************************************************************
 * 
 *  _   __ _     _____ _____                                            *
 * | | / /| |   |_   _/  __ \   This software is been developed by      *
 * | |/ / | |     | | | /  \/   a group of enthusiast hobbiest          *
 * |    \ | |     | | | |       with the purpose of learn and           *
 * | |\  \| |_____| |_| \__/\   have fun, so nobody is responsible or   *
 * \_| \_/\_____/\___/ \____/   will provide warranty.                  *
 *                                                                      *
 *  This file contains, structures, defines required for the
 *  irrigation Software
 * 
 * 
 * *********************************************************************/

// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>

// #include <NTPClient.h>
// #include <time.h>
// #include <TimeLib.h>
// #include <Timezone.h>


// Define NTP properties
#define NTP_OFFSET   60 * 60         // seconds
#define NTP_INTERVAL 60 * 1000       // miliseconds
#define NTP_ADDRESS  "pool.ntp.org"  // URL NTP Server


// save the converted epoc time
struct myTime_t
{
    byte hour;
    byte minute;
    byte second;
    unsigned int year;
    byte month;
    byte day;
};

