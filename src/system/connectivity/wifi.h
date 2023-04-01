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
 * Letś have Fun!!                                                      *
 *                                                                      *
 * ---------------------------------------------------------------------*/

#if !defined(__IRRIGATION_SYSTEM_CONNECTIVITY_WIFI_H__)
#define __IRRIGATION_SYSTEM_CONNECTIVITY_WIFI_H__

#include <WiFiManager.h>

/**
 * @brief Specifies the wifi ssid.
 * 
 */
extern const char* WIFI_SSID;

/**
 * @brief Specifies the wifi password.
 * 
 */
extern const char* WIFI_PASSWORD;

namespace WiFiConnection
{

    /**
     * @brief Initialize WIFI interface using the specified configuration.
     * 
     * @param wifi_ssid      [Optional] WiFi SSID as a const char string.
     * @param wifi_password  [Optional] WiFi Password as a const char string.
     * @return wl_status_t   WiFi status code.
     */
    wl_status_t WifiInitialize (
        const char* wifi_ssid = WIFI_SSID,
        const char* wifi_password = WIFI_PASSWORD
        );

};
#endif // !__IRRIGATION_SYSTEM_CONNECTIVITY_WIFI_H__
