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
#if !defined(__IRRIGATION_SYSTEM_RELAYS_IO_EXPANDER_H__)
#define __IRRIGATION_SYSTEM_RELAYS_IO_EXPANDER_H__
#define PCA9554_OUTPUT_REG1 1
#define PCA9554_CONFIG_REG3 3

#include <stdint.h>

/**
 * @brief Represents each of the IO entrys in the IO expander.
 * 
 */
typedef enum {
    IO_0,
    IO_1,
    IO_2,
    IO_3,
    IO_4,
    IO_5,
    IO_6,
    IO_7,
    IO_MAX
} IOActionPin_e;

class IOExpander
{
private:
    uint8_t ADDR;   //Address I2C value 7bits format    
   
    
public:
    /**
     * @brief Initializes the IOExpander
     * 
     * @param address I2C Address
     * @return true  On success
     * @return false On failure
     */
    bool init(uint8_t address);

    /**
     * @brief Writes a value in the IOExpander
     * 
     * @param position IO Position on the expander
     * @param state    Value to be written
     */
    bool write(IOActionPin_e position, bool state);

    /**
     * @brief 
     * 
     * @param position IO Position on the expander
     * @return true  Is HIGH
     * @return false Is LOW
     */
    bool read(IOActionPin_e position);
};

#endif // __IRRIGATION_SYSTEM_RELAYS_IO_EXPANDER_H__
