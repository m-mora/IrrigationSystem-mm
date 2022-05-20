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
    // Set the ESP as AccesPoint
    WiFi.mode(WIFI_STA);

    // TODO: Remove this code and use a general log library
    Serial.begin(115200);
    Serial.println();
    Serial.println("Conectando...");

    WiFiManager wm;

    // will try to connect when it has saved credentials
    // if it fails will start the access point
    if (wm.autoConnect(wifi_ssid,wifi_password)){
        Serial.print("Conectado a IP: ");
        Serial.println(WiFi.localIP());
    }
    
    return WL_CONNECTED;
}
