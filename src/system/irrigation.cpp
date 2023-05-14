#include "irrigation.h"

#include <Arduino.h>

#include "connectivity/wifi.h"
#include "relays/builder.h"
#include "utils/logger.h"
#include "system/connectivity/blynk.h"

SysLogger logger(nullptr);
Storage storage;

#define doUntilTimeElapsed(__time_handler, __ms_elapsed, __function) \
  static uint32_t __time_handler = 0;                                \
  if (__time_handler > millis())                                     \
  {                                                                  \
    __time_handler = millis();                                       \
  }                                                                  \
  if ((millis() - __time_handler) > __ms_elapsed)                    \
  {                                                                  \
    __time_handler = millis();                                       \
    __function                                                       \
  }

IrrigationSystem::IrrigationSystem()
{
  Status.Sensors.humidityLevel = 0.0f;
  Status.Sensors.isAnyValveOn = false;
  Status.Sensors.isPresenceDetected = false;
  Status.Sensors.isRaining = false;
}

bool IrrigationSystem::init()
{
  InitLogger();
  DumpSysInfo();

  logger << LOG_INFO << "Initializing System..." << EndLine;
  ConfigureNVRAM();
  InitWifi();
  ScanI2CDevicesAndDumpTable();
  InitDevices();
  ConfigureTimeProviders();
  InitSensors();
  InitRelays();
  InitDisplay();
  InitBlynk();
  InitTelegram();
  logger << LOG_INFO << "Initialization finished!" << EndLine;

  return IsSystemInitializedAtMinimal();
}

void IrrigationSystem::update() {
  Status.sysMilliseconds = millis();
  KlicBlynk::run();

  doUntilTimeElapsed(__update_rtc_handler, 1000, {
    timeProviders.update();
    logger << LOG_INFO << "Now:  " << timeProviders.get().toString()
            << EndLine;
    display.update(timeProviders.get().toString(), localBmp.get_temperature(), localBmp.get_pressure());
    WaterValve *relay = NULL;
    for (int i = 0; i < relays->size(); i++) {
      relay = relays->get(i);
      if (relay == NULL) {
        logger << LOG_WARN << "Found NULL instances while updating relays"
                << EndLine;
        continue;
      }
      relay->update();
    }
  });
}

void IrrigationSystem::DumpSysInfo()
{
  logger << LOG_MASTER << EndLine << LOGGER_TEXT_BOLD << LOGGER_TEXT_YELLOW
         << "--------------------------------------------------------\n"
         << "            _   __ _     _____ _____       \n"
         << "           | | / /| |   |_   _/  __ \\     \n"
         << "           | |/ / | |     | | | /  \\/     \n"
         << "           |    \\ | |     | | | |         \n"
         << "           | |\\  \\| |_____| |_| \\__/\\      \n"
         << "           \\_| \\_/\\_____/\\___/ \\____/     \n"
         << "--------------------------------------------------------\n"
         << LOG_MASTER << "Firmware version: " << FIRMWARE_VERSION << ", env: " << PIOENV << EndLine
         << LOG_MASTER << "Build date: " << Time_s(BUILD_TIME_UNIX).toString() << EndLine;
}

void IrrigationSystem::InitLogger()
{
  Serial.begin(KERNEL_SERIAL_SPEED);
  logger.setLogOutput(&Serial);
  logger.setLogLevel(LOG_LEVEL);
}

void IrrigationSystem::InitWifi() { WiFiConnection::WifiInitialize(); }

void IrrigationSystem::InitDevices()
{
  if (!ioExpander.init(0x38))
  {
    logger << LOG_ERROR << "Cant't initialize IO Expander" << EndLine;
  }
}

void IrrigationSystem::InitSensors() {
  localBmp.init();
}

void IrrigationSystem::InitRelays()
{
  logger << LOG_INFO << "Creating relay configurations" << EndLine;
  // TODO: Create a flexible interface for relay building from eeprom?
  relays = RelayCollectionBuilder::create()
               .setExpander(&ioExpander)
               .setSystemData(&Status)
               .setTimeProvider(&timeProviders)
               .setVoltageValvePin(IO_4)
               .forPin(IO_0)
               .onDays(ALL_WEEK)
               .onTime(20, 50, 0)
               .duration(20)
               .forPin(IO_1)
               .onDays(ALL_WEEK)
               .onTime(20, 53, 0)
               .duration(30)
               .forPin(IO_2)
               .onDays(ALL_WEEK)
               .onTime(20, 54, 0)
               .duration(35)
               .forPin(IO_3)
               .onDays(ALL_WEEK)
               .onTime(20, 55, 0)
               .duration(40)
               .done()
               ->build();
  // TODO: need to change the way it is saving once it is refactored
  if (!storage.getPrevSavedInfo())
  {
    storage.saveConfiguration(0, 20, 50, 0, 20, ALL_WEEK);
    storage.saveConfiguration(1, 20, 53, 0, 30, ALL_WEEK);
    storage.saveConfiguration(2, 20, 54, 0, 35, ALL_WEEK);
    storage.saveConfiguration(3, 20, 55, 0, 40, ALL_WEEK);
  }
  storage.dumpEEPROMValues();
  logger << LOG_INFO << LOGGER_TEXT_GREEN << "Done!" << EndLine;
}

void IrrigationSystem::ScanI2CDevicesAndDumpTable()
{
  uint8_t row = 0;
  char buffer[3];
  Wire.begin();
  I2CDevice test;

  logger << LOG_INFO << F("Scanning I2C Bus ") << EndLine;
  logger << LOG_INFO
         << F("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F")
         << EndLine;
  logger << LOG_INFO << F("  00: -- ");

  for (size_t i = 1; i < 0x80; i++)
  {
    test.setAddress(i);

    if (i % 16 == 0)
    {
      row += 0x10;
      sprintf(buffer, "%02X", row);
      logger << EndLine << LOG_INFO << F("  ") << buffer << F(": ");
    }

    if (test.isConnected())
    {
      sprintf(buffer, "%02X", i);
    }
    else
    {
      sprintf(buffer, "--");
    }

    logger << buffer << F(" ");
  }

  logger << EndLine;
}

void IrrigationSystem::ConfigureTimeProviders()
{
  timeProviders.TryToRegisterTimeProvider<TimeProviderNTP>();
  timeProviders.TryToRegisterTimeProvider<TimeProviderRTC>();

  timeProviders.init();
}

bool IrrigationSystem::IsSystemInitializedAtMinimal()
{
  if (timeProviders.countTimeProviders() == 0)
  {
    return false;
  }

  if (!ioExpander.isConnected())
  {
    return false;
  }

  return true;
}

void IrrigationSystem::InitDisplay()
{
  if (!display.init(0x3C))
  {
    logger << LOG_ERROR << "Can't initialize Display" << EndLine;
  }
}

void IrrigationSystem::ConfigureNVRAM()
{
  logger << LOG_INFO << "Initializing NVRAM Storage" << EndLine;
  storage.init(NVRAM_MAX_RELAYS);
  storage.dumpEEPROMValues();
}

void IrrigationSystem::InitBlynk()
{
  String t;
  logger << LOG_INFO << "Starting blynk" << EndLine;
  t = storage.getToken();
  char *token = new char[t.length() + 1];
  strcpy(token, t.c_str());
  KlicBlynk::init(token);
}

void IrrigationSystem::InitTelegram() {
  String telegram_token, chat_id, b_token;
  storage.getCredentials(chat_id,telegram_token,b_token);
  Telegram::init(telegram_token,chat_id);
}
