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

#define CHAT_ID_SIZE 10
#define TELEGRAM_TOKEN_SIZE 48
#define AUTH_TOKEN_SIZE 40
#define PADDING 2
typedef struct
{
  char _chatid[CHAT_ID_SIZE];
  char _telegramToken[TELEGRAM_TOKEN_SIZE];
  char _authToken[AUTH_TOKEN_SIZE];
  char _padding[PADDING];
} EEPROM_CredentialStorage_t;

#define SAVED_FLAG_START sizeof(EEPROM_CredentialStorage_t)
#define SAVED_FLAG_SIZE 4
#define SAVED_DATA_FLAG 0x3C
#define CONF_MEM_START SAVED_FLAG_START + SAVED_FLAG_SIZE // leave the first space for credentials and flags
                                                          // save the conf above this address

#pragma pack(push, 1)
typedef struct {
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint32_t duration;
  uint8_t days;
} eeprom_map_conf_time_t;
#pragma pack(pop)

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
  bool saveConfiguration(int relayID, uint8_t hour, uint8_t minute, uint8_t second, uint32_t duration, uint8_t days);
  bool getCredentials(String& chatId, String& telegramToken, String& authToken);
  bool getPrevSavedInfo();
  void setPrevSavedInfo();
  eeprom_map_conf_time_t getConfiguration(int relayID);
  String getToken();

  void dumpEEPROMValues();
};

#endif //__IRRIGATION_SYSTEM_STORAGE_STORAGE_H__
