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
#if !defined(__IRRIGATION_RELAYS_IO_RELAY_H__)
#define __IRRIGATION_RELAYS_IO_RELAY_H__

#include "action.h"
#include "relay_config.h"

class IORelay : public IOAction
{
    IORelayConfig_t config;
public:
    /**
     * @brief Construct a new IORelay object
     * 
     * @param position IO Position on the expander.
     * @param config   Relay configuration
     */
    IORelay(IOActionPin_e position, IORelayConfig_t config)
        : IOAction(position), config(config) {}

    /**
     * @brief Set the config object
     * 
     * @param new_config New configuration for the relay.
     */
    void set_config (IORelayConfig_t new_config);

    /**
     * @brief Update the relay state.
     * 
     */
    void update();
};

#endif // __IRRIGATION_RELAYS_IO_RELAY_H__
