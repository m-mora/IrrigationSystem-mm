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
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "utils/logger.h"
#include "telegram.h"


using namespace Telegram;

WiFiClientSecure secured_client;
UniversalTelegramBot bot("DUMMY TOKEN", secured_client);

String ChatID;

void Telegram::init(String token, String _id) {
  secured_client.setInsecure();
  bot.updateToken(token);
  ChatID = _id;
  bot.sendMessage(ChatID,"Initializing Telegram in Irrigation System", "");
  logger << LOG_DEBUG << "Get token " << bot.getToken() << EndLine;
}

void Telegram::message(String m) {
  bot.sendMessage(ChatID,m, "");
}