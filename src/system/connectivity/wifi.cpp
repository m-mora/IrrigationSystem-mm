#include "wifi.h"
#include "utils/logger.h"
// #ifdef ARDUINO_ESP8266_ESP12
// #include <ESP8266WiFi.h>
// #else
//#include <WiFi.h>
//#endif

static WiFiManager wm;
using namespace WiFiConnection;

    static String templateID;
    static String deviceName;
    static String token;

static void saveParamCallback()
{
    if(wm.server->hasArg("template_id"))
    {
        templateID = wm.server->arg("template_id");
        logger << LOG_INFO << "TemplateID = " << templateID << EndLine;
    }
    if(wm.server->hasArg("device_name"))
    {
        deviceName = wm.server->arg("device_name");
        logger << LOG_INFO << "Device Name = " << deviceName << EndLine;
    }
    if(wm.server->hasArg("auth_token"))
    {
        token = wm.server->arg("auth_token");
        logger << LOG_INFO << "Token = " << token << EndLine;
    }
    return;
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

    WiFiManagerParameter templateID("template_id", "TEMPLATE_ID", "template_id",20);
    WiFiManagerParameter deviceName("device_name", "DEVICE_NAME", "Device_Name", 20);
    WiFiManagerParameter authToken("auth_token", "AUTH_TOKEN", "auth_token", 20);

    wm.addParameter(&templateID);
    wm.addParameter(&deviceName);
    wm.addParameter(&authToken);

    wm.setSaveParamsCallback(saveParamCallback);

    wm.setTitle("Irrigation System");
    std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
    wm.setMenu(menu);
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


