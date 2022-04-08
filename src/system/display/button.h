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
#if !defined(__IRRIGATION_SYSTEM_DISPLAY_BUTTON_H__)
#define __IRRIGATION_SYSTEM_DISPLAY_BUTTON_H__

#include <stdint.h>

class DigitalButton
{
    int8_t pin;
    bool state;
public:
    /**
     * @brief Construct a new Digital Button object
     * 
     */
    DigitalButton()
        : pin(-1), state(false) {}

    /**
     * @brief Initialize digital button
     * 
     * @return true  On success 
     * @return false On failure
     */
    bool init();

    /**
     * @brief Update current state of digital button.
     * 
     * @return true  On success 
     * @return false On failure
     */
    bool update();

    /**
     * @brief Wait a specified amount of time for digital button
     *        get active.
     * 
     * @param timeout Max amount of time to wait in milliseconds.
     * @return true  If button is pressed.
     * @return false Button were not pressed.
     */
    bool wait(uint32_t timeout);

    /**
     * @brief Attach button to pin.
     * 
     * @param pin Pin number.
     */
    void attach_pin (uint8_t pin);

    /**
     * @brief Get current state of digital button.
     * 
     * @return true  Pressed.
     * @return false Not pressed.
     */
    bool is_pressed() const {
        return this->state;
    }
};

#endif // __IRRIGATION_SYSTEM_DISPLAY_BUTTON_H__
