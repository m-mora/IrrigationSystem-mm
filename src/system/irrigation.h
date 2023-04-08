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
#if !defined(__IRRIGATION_SYSTEM_H__)
#define __IRRIGATION_SYSTEM_H__

#include "system/time/itime_provider.h"
#include "system_data.h"
#include "system/relays/io_expander.h"
#include "system/relays/water_valve.h"
#include "time/services/ntp.h"
#include "time/services/rtc.h"
#include "utils/list.h"
#include "utils/logger.h"
#include "system/connectivity/wifi.h"
#include "time/controller.hpp"
#include "utils/storage.h"
#include "system/display/display.h"

#define KERNEL_SERIAL_SPEED 115200
#define NVRAM_MAX_RELAYS    4

class IrrigationSystem {
    //
    // Components
    //
    SystemTimeProvider timeProviders;
    IOExpander ioExpander;
    Display display;
    LinkedList<WaterValve*> *relays;
    //
    // Private methods
    //
    void DumpSysInfo();
    void InitLogger();
    void InitWifi();
    void InitDevices();
    void InitSensors();
    void InitRelays();
    void InitDisplay();
    void ConfigureTimeProviders();
    void ConfigureNVRAM();

    void ScanI2CDevicesAndDumpTable();

    bool IsSystemInitializedAtMinimal ();

    //
    // Data members
    //
    SystemData_t Status;
public:
    IrrigationSystem();
    bool init();
    void run();
};

#endif // __IRRIGATION_SYSTEM_H__
