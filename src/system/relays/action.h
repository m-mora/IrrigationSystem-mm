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
#if !defined(__IRRIGATION_SYSTEM_RELAYS_ACTION_H__)
#define __IRRIGATION_SYSTEM_RELAYS_ACTION_H__

#include "io_expander.h"

class IOAction {
protected:
    IOExpander *device;
    IOActionPin_e bit;
    bool state;
public:
    /**
     * @brief Construct a new IOAction object
     * 
     * @param position IO Position on the expander.
     */
    IOAction(IOActionPin_e position)
        : device(nullptr), bit(position), state(false) {}

    /**
     * @brief Initializes the IOAction
     * 
     * @return true  On success
     * @return false On failure
     */
    bool init ();

    /**
     * @brief Updates the state.
     * 
     * @return true  On success
     * @return false On failure
     */
    bool update ();

    /**
     * @brief Attach action to a IO Expander.
     * 
     * @param expander Pointer to IO Expander object.
     */
    void attach_expander (IOExpander* expander);

    /**
     * @brief Returns the state.
     * 
     * @return bool the state of the action.
     */
    bool active () const {
        return this->state;
    }
};

#endif // __IRRIGATION_SYSTEM_RELAYS_ACTION_H__
