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
#if !defined(__IRRIGATION_SYSTEM_SENSORS_LIST_H__)
#define __IRRIGATION_SYSTEM_SENSORS_LIST_H__

#include <stdint.h>
#include "isensor.h"
#include "utils/list.h"

/**
 * @brief Type of sensor identifier, used for interaction with list.
 * 
 */
typedef uint8_t SensorId_t;

struct MapIdSensor_t {
    /**
     * @brief Sensor id on the sensor list.
     * 
     */
    SensorId_t  id;
    ISensor*    device;
};

typedef struct MapIdSensor_t MapIdSensor_t;

class SensorList
{
    SensorId_t last_id;
    LinkedList<MapIdSensor_t> sensors;
public:
    SensorList()
        : last_id(0), sensors() {}

    /**
     * @brief Initializes the current list of sensors.
     * 
     * @return true  On success 
     * @return false On failure
     */
    bool init();

    /**
     * @brief Updates the sensor value.
     * 
     * @return true  On success 
     * @return false On failure
     */
    bool update();

    /**
     * @brief Read the value from the sensor id, returns -1 if not found.
     * 
     * @return int Sensor value.
     */
    int read (SensorId_t);

    /**
     * @brief Adds a new sensor to the list.
     * 
     * @tparam SensorType   Sensor class.
     * @param name          Sensor name
     * @param attach_pin   Pin where is attached.
     * @return SensorId_t   Sensor id.
     */
    template <class SensorType>
    SensorId_t add(const char* name, int attach_pin) {
        last_id ++;
        ISensor* sensor = new SensorType (name, attach_pin);
        this->sensors.add ({last_id, sensor});
    }

    /**
     * @brief Destroy the Sensor List object
     * 
     */
    ~SensorList () {
        //
        // Free all memory before exit.
        //
        for_in_range (size_t, cont, 0, this->sensors.size()) {
            ISensor* sensor = this->sensors[cont].device;
            if (sensor != nullptr) {
                delete sensor;
            }
        }
    }
};

#endif // __IRRIGATION_SYSTEM_SENSORS_LIST_H__
