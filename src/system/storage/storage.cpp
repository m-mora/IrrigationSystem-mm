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

#include "storage.h"
#include "utils/mem.h"

static String templateId;
static String templateName;
static String authToken;

Storage::Storage() {}

Storage::~Storage() {}

void Storage::init(int _num_relays)
{
  this->num_relays = _num_relays;
  // offset for credentials, plus space requred to save the configuration for each relay
  allocatedMemory = CONF_MEM_START + (sizeof(eeprom_map_conf_time_t) * _num_relays);
  startAddress = 0;
  EEPROM.begin(allocatedMemory);
  // Clean up the EEPROM space if there was no information saved previously
  for (size_t address = startAddress; address < allocatedMemory && !getPrevSavedInfo(); address++)
  {
    EEPROM.write(address, 0x00);
  }
  EEPROM.commit();
}

/*
 * Save them in the simulated EEPROM
 * |------------| 0x0
 * | templateID |
 * |------------|
 * | templ name |
 * |------------|
 * | auth_token |
 * |------------|
 */
bool Storage::saveCredentials(String chatID, String telegramToken, String authToken) {
  EEPROM_CredentialStorage_t d;
  ZeroMem(&d, sizeof(EEPROM_CredentialStorage_t));
  strcpy(d._chatid, chatID.c_str());
  strcpy(d._telegramToken, telegramToken.c_str());
  strcpy(d._authToken, authToken.c_str());
  EEPROM.put(offsetof(EEPROM_CredentialStorage_t, _chatid), d);

  EEPROM.commit();
  logger << LOG_INFO << "Credentials saved" << EndLine;

  return true;
}

bool Storage::saveConfiguration(int relayID, uint8_t hour, uint8_t minute, uint8_t second, uint32_t duration, uint8_t days) {
  relayConfigTime.hour = hour;
  relayConfigTime.min = minute;
  relayConfigTime.sec = second;
  relayConfigTime.duration = duration;
  relayConfigTime.days = days;

  EEPROM.put(CONF_MEM_START + (sizeof(eeprom_map_conf_time_t) * relayID), relayConfigTime);
  EEPROM.commit();

  return true;
}

bool Storage::getCredentials(String& chatId, String& telegramToken, String& authToken) {
  EEPROM_CredentialStorage_t d;
  // Check if info was saved previously, if not return empty strings
  if (getPrevSavedInfo())
  {
    // Credentials are on the bottom of the reserved memory, use address 0
    EEPROM.get(0, d);
    chatId = d._chatid;
    telegramToken = d._telegramToken;
    authToken = d._authToken;

    logger << LOG_DEBUG << "chatId " << chatId << EndLine;
    logger << LOG_DEBUG << "telegramToken " << telegramToken << EndLine;
    logger << LOG_DEBUG << "authToken " << authToken << EndLine;
  }
  else
  {
    // if not saved credentials previously, left the parameters clean
    chatId = "";
    telegramToken = "";
    authToken = "";
  }
  return true;
}

eeprom_map_conf_time_t Storage::getConfiguration(int relayID)
{
  EEPROM.get(CONF_MEM_START + (sizeof(eeprom_map_conf_time_t) * relayID), relayConfigTime);

  return relayConfigTime;
}

bool Storage::getPrevSavedInfo()
{
  uint8_t isSaved = 0;
  EEPROM.get(SAVED_FLAG_START, isSaved);
  logger << LOG_DEBUG << "Checking saved flag = " << INT_HEX << isSaved << EndLine;
  return (SAVED_DATA_FLAG == isSaved);
}

void Storage::setPrevSavedInfo()
{
  EEPROM.put(SAVED_FLAG_START, SAVED_DATA_FLAG);
  EEPROM.commit();
  logger << LOG_INFO << "Setting saved flag - " << INT_HEX << SAVED_DATA_FLAG << EndLine;
}

void Storage::dumpEEPROMValues()
{
  logger << LOG_INFO << F("Dumping EEPROM Data") << EndLine;
  logger << LOG_INFO << F("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10 11 12 13") << EndLine;
  logger << LOG_INFO << F("  00: ");
  size_t row = 0;
  char buffer[3];

  for (size_t address = startAddress; address < allocatedMemory; address++)
  {
    uint8_t data = EEPROM.read(address);

    if (address > 0 && address % 20 == 0)
    {
      row += 0x14;
      sprintf(buffer, "%02X", row);
      logger << EndLine << LOG_INFO << F("  ") << buffer << F(": ");
    }

    sprintf(buffer, "%02X", data);
    logger << buffer << F(" ");
  }

  logger << EndLine;
}

String Storage::getToken()
{
  EEPROM_CredentialStorage_t d;
  // Check if info was saved previously, if not return empty strings
  if (getPrevSavedInfo())
  {
    // Credentials are on the bottom of the reserved memory, use address 0
    EEPROM.get(0, d);
    authToken = d._authToken;
  }
  else
  {
    authToken = "";
  }
  return authToken;
}
