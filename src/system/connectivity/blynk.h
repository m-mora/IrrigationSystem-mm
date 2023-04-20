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
 * Let's have Fun!!                                                      *
 *                                                                      *
 * ---------------------------------------------------------------------*/
#if !defined(__SYSTEM_CONNECTIVITY_BLYNK_H__)
#define __SYSTEM_CONNECTIVITY_BLYNK_H__

#include <Arduino.h>
#include "system/time/datetime.h"
#include "utils/logger.h"

#define BLYNK_PRINT Serial

#define RELAY1 0
#define RELAY2 1
#define RELAY3 2
#define RELAY4 3

#define SECONDS_IN_A_MINUTE 60

#pragma pack(push, 1)
typedef union
{
  struct
  {
    uint32_t monday : 1;
    uint32_t tuesday : 1;
    uint32_t wednesday : 1;
    uint32_t thursday : 1;
    uint32_t friday : 1;
    uint32_t saturday : 1;
    uint32_t sunday : 1;
    uint32_t hour : 5;
    uint32_t min : 6;
    uint32_t duration : 11;
    uint32_t _ : 3;
  };
  uint32_t data;
} dev_conf_t;

#pragma pack(pop)

namespace KlicBlynk
{
  void init(const char *_auth);
  void run();
  void showStatus(int r, bool s);
}

#endif // __SYSTEM_CONNECTIVITY_BLYNK_H__
