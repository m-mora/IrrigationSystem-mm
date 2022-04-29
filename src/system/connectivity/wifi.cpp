#include "wifi.h"

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
    return WL_CONNECTED;
}
