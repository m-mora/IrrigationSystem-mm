
#include <Arduino.h>

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

//Define Ports connections
#define PIR             0
#define RAIN_SENSOR     2
#define SDA             4
#define SCL             5
#define LEFT            12
#define RIGTH           13
#define DOWN            14
#define ENTER           15
#define UP              16
#define HUMIDITY        A0

// save the converted epoc time
typedef struct 
{
    byte hour;
    byte minute;
    byte second;
    unsigned int year;
    byte month;
    byte day;
} myTime_t;
