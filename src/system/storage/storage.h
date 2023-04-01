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
 * Let's have Fun!!                                                     *
 *                                                                      *
 * ---------------------------------------------------------------------*/

#if !defined(__IRRIGATION_SYSTEM_STORAGE_STORAGE_H__)
#define __IRRIGATION_SYSTEM_STORAGE_STORAGE_H__
#include <Arduino.h>
#include <EEPROM.h>
#include <stddef.h>
#include <stdint.h>
#include "../../utils/logger.h"

#define TEMPLATE_ID_SIZE    20
#define TEMPLATE_NAME_SIZE  20
#define AUTH_TOKEN_SIZE     40
#define PADDING             20
typedef struct {
  char _templateid [TEMPLATE_ID_SIZE];
  char _templateName [TEMPLATE_NAME_SIZE];
  char _authToken [AUTH_TOKEN_SIZE];
  char _padding [PADDING];
} EEPROM_CredentialStorage_t;

#define SAVED_FLAG_START  sizeof(EEPROM_CredentialStorage_t)
#define SAVED_FLAG_SIZE   4
#define SAVED_DATA_FLAG   0x3C
#define CONF_MEM_START    SAVED_FLAG_START + SAVED_FLAG_SIZE    // leave the first space for credentials and flags
                                                                // save the conf above this address

typedef struct {
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t duration;
  uint8_t days;
} eeprom_map_conf_time_t;

class Storage
{
private:
  int num_relays;
  eeprom_map_conf_time_t relayConfigTime;
  size_t allocatedMemory;
  size_t startAddress;

public:
  Storage();
  ~Storage();
  void init(int _num_relays);
  bool saveCredentials(String templateID, String templateName, String authToken);
  bool saveConfiguration(int relayID, uint8_t hour, uint8_t minute, uint8_t second, uint8_t duration, uint8_t days);
  bool getCredentials(String templateID, String templateName, String authToken);
  bool getConfiguration(int relayID, uint8_t hour, uint8_t minute, uint8_t second, uint8_t duration, uint8_t days);
  bool getPrevSavedInfo();
  void setPrevSavedInfo();

  void dumpEEPROMValues();
};

#endif //__IRRIGATION_SYSTEM_STORAGE_STORAGE_H__
