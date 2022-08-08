#include "wifi.h"
#include "utils/logger.h"

static WiFiManager wm;

/**
 * @brief Initialize WIFI interface using the specified configuration.
 * 
 * @param wifi_ssid      [Optional] WiFi SSID as a const char string.
 * @param wifi_password  [Optional] WiFi Password as a const char string.
 * @return wl_status_t   WiFi status code.
 */
wl_status_t WifiInitialize (
    const char* wifi_ssid,
    const char* wifi_password
    )
{
    logger << LOG_INFO << "Initializing WiFi provider!" << EndLine;

    // Set the ESP as AccesPoint
    WiFi.mode(WIFI_STA);

    // will try to connect when it has saved credentials
    // if it fails will start the access point
    if (wm.autoConnect(wifi_ssid,wifi_password)){
        logger << LOG_INFO << "WiFi AutoConnect done! IP: " << WiFi.localIP() << EndLine;
    }

    return WL_CONNECTED;
}
