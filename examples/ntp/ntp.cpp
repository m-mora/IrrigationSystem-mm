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
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <NTPClient.h>
#include <time.h>
#include <TimeLib.h>
#include <Timezone.h>


/*
 * Project files
 */
#include "secret.h"
#include "irrigationsystem.h" 

#define DEBUG 1     // use this define to enable features to debug
                    // like prints

/*
 * Global variables
 */
myTime_t myTime;    // define an estructure to hold the time and date
time_t local, utc;  // store local time and Universal Time Coordinated (UTC)

// Configure NTP UDP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

//  Guadalajara Time zone
// and initial and end of the day saving
// TODO: there should be a better way to manage day saving dates, not too manual (hard coded).
TimeChangeRule mxCDT = {"CDT", First, Sun, Apr, 2, -360};    // Guadalajara day saving 
TimeChangeRule mxCST = {"CST ", Last, Sun, Oct, 2, -420};    // Guadalajara standar time
Timezone gm(mxCDT, mxCST);

/************************************************************************
 *  This function is used to initialize all required objects, structs
 *  it executus one time at the begining of the program.
 * *********************************************************************/
void setup()
{
    // Initialize serial port
    Serial.begin(115200);

    // Connect to wifi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
#if DEBUG
    Serial.setDebugOutput(true);
    Serial.println("");
    Serial.print("Connected to IP ");
    Serial.print(WiFi.localIP());
    Serial.println("");
    Serial.setDebugOutput(false);
#endif

    // initialize NTPClient
    timeClient.begin();


}

/************************************************************************
 *  Here is the main function
 * **********************************************************************/
void loop()
{

}


/*----------------------------------------------------------------------*
 * This function get the time from the NTP server if available or from 
 * a real time clock and fill up an structure with current date 
 * the structure is a global variable
 * Parameters: void
 * return: void
*----------------------------------------------------------------------*/

void getTime()
{

    if (WiFi.status() == WL_CONNECTED) // Checking the WIFI connection
    {   
     
    // Update NTP CLient in UNIX UTC
    timeClient.forceUpdate();
    timeClient.update();
    unsigned long utc =  timeClient.getEpochTime();
    int attempts = 0;
    // this is a workaround to a bad data obtained
    if (utc < 1630373993 && attempts < 5)
    {
      timeClient.update();
      attempts++;
    }
    
    // Convert to local time
    local = gm.toLocal(utc);
    setTime(local);
    myTime.day = day();
    myTime.month = month();
    myTime.year = year();
    myTime.hour = hour();
    myTime.minute = minute();
    myTime.second = second();

    // Update the RTC
        // Need to add code here
#if DEBUG
    char buff[40];
    sprintf(buff,"%02d/%02d/%04d %02d:%02d:%02d",myTime.day,myTime.month,myTime.year,myTime.hour, myTime.minute,myTime.second);
    Serial.print("[DEBUG] "); Serial.println(buff);

#endif
    }
    else // If lost connection use RTC
    {
        // implement RTC code
#if DEBUG
        Serial.println("using RTC to get time");
#endif
    }
    return;
}
