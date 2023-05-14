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
 * Let's have Fun!!                                                      *
 *                                                                      *
 * ---------------------------------------------------------------------*/
#ifndef __SYSTEM_SENSORS_BMP_H__
#include <Arduino.h>
#include <Adafruit_BMP085.h>


class bmpSensor {
private:
  int temperature = 0;
  int pressure = 0;

public:
  bool init();
  float get_temperature();
  uint32_t get_pressure();
};

#endif // !__SYSTEM_SENSORS_BMP_H__
