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
#if !defined(__IRRIGATION_SYSTEM_RELAYS_CONFIGURATION_H__)
#define __IRRIGATION_SYSTEM_RELAYS_CONFIGURATION_H__

#include "system/time/datetime.h"
#include "system/time/itime_provider.h"
#include "system/system_data.h"

typedef struct {
    //
    // Pointers to access system structures
    //
    ITimeProvider   *timeProvider;
    SystemData_t    *systemData;
    IOAction        *voltageRelay;

    //
    // Info for relay
    //
    Time_s timeToTurnOn;
    uint16_t turnOnDuration;
    union {
        struct {
            uint8_t sunday      : 1;
            uint8_t monday      : 1;
            uint8_t tuesday     : 1;
            uint8_t wensday     : 1;
            uint8_t thursday    : 1;
            uint8_t friday      : 1;
            uint8_t saturnday   : 1;
            uint8_t _           : 1;
        } Days;
        uint8_t Data;
    } WeekDaysToTurnOn;
} IORelayConfig_t;

#endif // __IRRIGATION_SYSTEM_RELAYS_CONFIGURATION_H__
