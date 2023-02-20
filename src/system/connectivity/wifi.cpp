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

/**
 * @brief Initialize WIFI interface using the specified configuration.
 * 
 * @param wifi_ssid      [Optional] WiFi SSID as a const char string.
 * @param wifi_password  [Optional] WiFi Password as a const char string.
 * @return wl_status_t   WiFi status code.
 */
static wl_status_t WifiInitialize (
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

    // Set the ESP as AccesPoint
    WiFi.mode(WIFI_STA);

    // will try to connect when it has saved credentials
    // if it fails will start the access point
    if (wm.autoConnect(wifi_ssid,wifi_password)){
        logger << LOG_INFO << "WiFi AutoConnect done! IP: " << WiFi.localIP() << EndLine;
    }

    return WL_CONNECTED;
}

static String getDeviceName()
{
    return deviceName;
}

static String getTemplate()
{
    return templateID;
}

static String getToken()
{
    return token;
}

static void saveParamCallback()
{
    if(wm.server->hasArg("template_id"))
    {
        templateID = wm.server->arg("template_id");
    }
    if(wm.server->hasArg("device_name"))
    {
        deviceName = wm.server->arg("device_name");
    }
    if(wm.server->hasArg("auth_token"))
    {
        token = wm.server->arg("auth_token");
    }
    return;
}

