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
#if !defined(__IRRIGATION_SYSTEM_DISPLAY_USER_INPUT_H__)
#define __IRRIGATION_SYSTEM_DISPLAY_USER_INPUT_H__

#include "button.h"

typedef enum {
    KEY_LEFT,   // Left
    KEY_RIGHT,  // Right
    KEY_UP,     // Up
    KEY_DOWN,   // Down
    KEY_ENTER,  // Enter
    KEY_NUM     // No key
} ControllerKeys_e;

class KeysController {
    struct MapKeyToButton {
        ControllerKeys_e key;
        DigitalButton button;
    };

    /**
     * @brief Map between keys and buttons.
     * 
     */
    MapKeyToButton buttons[KEY_NUM];

    /**
     * @brief Represents the current key pressed.
     * 
     */
    ControllerKeys_e current_state;
public:
    /**
     * @brief Construct a new Keys Controller object
     * 
     */
    KeysController();

    /**
     * @brief Set pin for the button.
     * 
     * @param key Key of the button
     * @param pin Pin to attach.
     */
    void map (ControllerKeys_e key, uint8_t pin);

    /**
     * @brief Initialize controller with current configuration.
     * 
     * @return true  On success 
     * @return false On failure
     */
    bool init();

    /**
     * @brief Updates the current key pressed.
     * 
     * @return true  On success 
     * @return false On failure
     */
    bool update();

    /**
     * @brief Get the current key pressed.
     * 
     * @return ControllerKeys_e Current key.
     */
    ControllerKeys_e get_current () const {
        return this->current_state;
    }
};

#endif // __IRRIGATION_SYSTEM_DISPLAY_USER_INPUT_H__
