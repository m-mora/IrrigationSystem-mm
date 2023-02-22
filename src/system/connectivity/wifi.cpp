#include "wifi.h"
#include "utils/logger.h"

#define WIFI_MANAGER_PARAMETER_SIZE 20

static WiFiManager wm;
using namespace WiFiConnection;

static String templateID;
static String deviceName;
static String token;

static void setPreSaveConfigCallback()
{
    if(wm.server->hasArg("template_id"))
    {
        templateID = wm.server->arg("template_id");
        logger << LOG_INFO << "TemplateID = " << templateID << EndLine;
    }
    else 
    {
        logger << LOG_INFO << "Template ID missing" << EndLine;
    }
    if(wm.server->hasArg("device_name"))
    {
        deviceName = wm.server->arg("device_name");
        logger << LOG_INFO << "Device Name = " << deviceName << EndLine;
    }
    else 
    {
        logger << LOG_INFO << "Device Name missing" << EndLine;
    }
    if(wm.server->hasArg("auth_token"))
    {
        token = wm.server->arg("auth_token");
        logger << LOG_INFO << "Token = " << token << EndLine;
    }
    else 
    {
        logger << LOG_INFO << "Auth_Token missing" << EndLine;
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
    logger << LOG_INFO << "Initializing WiFi provider!" << EndLine;

    WiFiManagerParameter templateID("template_id", "TEMPLATE_ID", "template_id",WIFI_MANAGER_PARAMETER_SIZE);
    WiFiManagerParameter deviceName("device_name", "DEVICE_NAME", "Device_Name", WIFI_MANAGER_PARAMETER_SIZE);
    WiFiManagerParameter authToken("auth_token", "AUTH_TOKEN", "auth_token", WIFI_MANAGER_PARAMETER_SIZE);

    wm.addParameter(&templateID);
    wm.addParameter(&deviceName);
    wm.addParameter(&authToken);

    wm.setPreSaveConfigCallback(setPreSaveConfigCallback);

    wm.setTitle("Irrigation System");
    const char *menu[] = {"wifi","info","param","sep","restart","exit"};
    wm.setMenu(menu,6);
    wm.setClass("invert");

    // wm.resetSettings();
    // Set the ESP as AccesPoint
    WiFi.mode(WIFI_STA);

    // will try to connect when it has saved credentials
    // if it fails will start the access point
    if (wm.autoConnect(wifi_ssid,wifi_password)){
        logger << LOG_INFO << "WiFi AutoConnect done! IP: " << WiFi.localIP() << EndLine;
    }

    return WL_CONNECTED;
}

String WiFiConnection::getDeviceName()
{
    return deviceName;
}

String WiFiConnection::getTemplate()
{
    return templateID;
}

String WiFiConnection::getToken()
{
    return token;
}


