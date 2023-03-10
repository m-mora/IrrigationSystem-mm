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
#if !defined(__IRRIGATION_SYSTEM_TIME_INTERFACE_H__)
#define __IRRIGATION_SYSTEM_TIME_INTERFACE_H__

#include "datetime.h"

class ITimeProvider {
protected:
    Time_s datetime;
public:
    typedef enum {
        PRIMARY,
        BACKUP
    } Type_e;

    /**
     * @brief Initialize the time provider.
     * 
     * @return true  On success
     * @return false On failure
     */
    virtual bool init() = 0;

    /**
     * @brief Update the internal datetime.
     * 
     * @return true  On success
     * @return false On failure
     */
    virtual bool update() = 0;

    /**
     * @brief Returns a copy of the internal datetime.
     * 
     * @return Time_s Last updated datetime.
     */
    Time_s get() const { return datetime; }

    virtual void set(Time_s newNow) {  }

    virtual const char* getTypeName () const = 0;

    virtual ~ITimeProvider() {}

    Type_e getType() const { return _type; }

protected:
    Type_e _type;
};

#endif // __IRRIGATION_SYSTEM_TIME_INTERFACE_H__
