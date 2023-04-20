#include "wifi.h"
#include "utils/logger.h"
#include "utils/storage.h"

#define WIFI_MANAGER_CHATID_SIZE    10
#define WIFI_MANAGER_AUTHTOKEN_SIZE 40
#define WIFI_TELEGRAM_TOKEN_SIZE    48

static WiFiManager wm;
using namespace WiFiConnection;

static String chat_id;
static String telegram_token;
static String token;

static void saveConfigCallback()
{
    if((wm.server->hasArg("chat_id")) && (wm.server->hasArg("telegram_token")) && (wm.server->hasArg("auth_token")))
    {
        chat_id = wm.server->arg("chat_id");
        telegram_token = wm.server->arg("telegram_token");
        token = wm.server->arg("auth_token");
        logger << LOG_INFO << "chat_id = " << chat_id << EndLine;
        logger << LOG_INFO << "telegram_token = " << telegram_token << EndLine;
        logger << LOG_INFO << "Token = " << token << EndLine;

        storage.saveCredentials(chat_id,telegram_token,token);
        storage.setPrevSavedInfo();
    } else {
        logger << LOG_ERROR << "At least one of the configuration is missing" << EndLine;
        logger << LOG_ERROR << "chat_id " << wm.server->hasArg("chat_id") << EndLine;
        logger << LOG_ERROR << "telegram_token " << wm.server->hasArg("telegram_token") << EndLine;
        logger << LOG_ERROR << "Token " << wm.server->hasArg("auth_token") << EndLine;
    }

}

/**
 * @brief Initialize WIFI interface using the specified configuration.
 * 
 * @param wifi_ssid      [Optional] WiFi SSID as a const char string.
 * @param wifi_password  [Optional] WiFi Password as a const char string.
 * @return wl_status_t   WiFi status code.
 */
wl_status_t WiFiConnection::WifiInitialize (
    const char* wifi_ssid,
    const char* wifi_password
    )
{
    String chat_id = "";
    String telegram_token = "";
    String auth_token = "";
    // if there was not saved information, will return clean strings
    storage.getCredentials(chat_id,telegram_token,auth_token);

    logger << LOG_INFO << "Initializing WiFi provider!" << EndLine;
    // parameters  = ID, label, default_value, size
    WiFiManagerParameter chatId("chat_id", "CHAT_ID", chat_id.c_str(), WIFI_MANAGER_CHATID_SIZE);
    WiFiManagerParameter telegramToken("telegram_token", "TELEGRAM_TOKEN", telegram_token.c_str(), WIFI_TELEGRAM_TOKEN_SIZE);
    WiFiManagerParameter authToken("auth_token", "AUTH_TOKEN", auth_token.c_str(), WIFI_MANAGER_AUTHTOKEN_SIZE);

    wm.addParameter(&chatId);
    wm.addParameter(&telegramToken);
    wm.addParameter(&authToken);

    wm.setSaveParamsCallback(saveConfigCallback);

    wm.setTitle("Irrigation System");
    const char *menu[] = {"wifi","info","param","sep","restart","exit"};
    wm.setMenu(menu,6);
    wm.setClass("invert");

    if(!storage.getPrevSavedInfo()) {
        // if the credentials were not set, reset all wifi configuration
        wm.resetSettings();
    }
#ifdef RESET_WIFI
    wm.resetSettings();
#endif
    // Set the ESP as AccesPoint
    WiFi.mode(WIFI_STA);

    wm.setConnectTimeout(20);
    wm.setConfigPortalTimeout(30);
    // will try to connect when it has saved credentials
    // if it fails will start the access point
    if (wm.autoConnect(wifi_ssid,wifi_password)){
        logger << LOG_INFO << "WiFi AutoConnect done! IP: " << WiFi.localIP() << EndLine;
    }

    return WL_CONNECTED;
}
