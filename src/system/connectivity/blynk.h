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

#pragma pack(push, 1)
typedef union
{
  struct
  {
    uint32_t lunes : 1;
    uint32_t martes : 1;
    uint32_t miercoles : 1;
    uint32_t jueves : 1;
    uint32_t viernes : 1;
    uint32_t sabado : 1;
    uint32_t domingo : 1;
    uint32_t hora : 5;
    uint32_t min : 6;
    uint32_t duration : 4;
    uint32_t _ : 10;
  };
  uint32_t data;
} dev_conf_t;

#pragma pack(pop)

namespace KlicBlynk
{
  void init(const char *_auth);
  void run();
}

#endif // __SYSTEM_CONNECTIVITY_BLYNK_H__
