#include "blynk.h"

#if ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>
#elif ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#else
#error "Unsupported ARDUINO architecture"
#endif

#include "utils/mem.h"
#include "utils/logger.h"
#include "utils/storage.h"

static dev_conf_t temp_inf;
static uint8_t temp_relay;

void KlicBlynk::init(const char *_auth)
{
  Blynk.config(_auth);
}

void KlicBlynk::run()
{
  Blynk.run();
}

void KlicBlynk::showStatus(int relay,bool status)
{
  switch(relay)
  {
    case RELAY1: //turn on relay 1
      Blynk.virtualWrite(V7,status);
      Blynk.virtualWrite(V8,0);
      Blynk.virtualWrite(V9,0);
      break;
    case RELAY2:
      Blynk.virtualWrite(V7,0);
      Blynk.virtualWrite(V8,status);
      Blynk.virtualWrite(V9,0);
      break;
    case RELAY3:
      Blynk.virtualWrite(V7,status);
      Blynk.virtualWrite(V8,status);
      Blynk.virtualWrite(V9,0);
      break;
    case RELAY4:
      Blynk.virtualWrite(V7,0);
      Blynk.virtualWrite(V8,0);
      Blynk.virtualWrite(V9,status);
      break;
    default:
      Blynk.virtualWrite(V7,0);
      Blynk.virtualWrite(V8,0);
      Blynk.virtualWrite(V9,0);
  }
}

dev_conf_t getConfFromMem(uint8_t _relay)
{
  dev_conf_t t;
  eeprom_map_conf_time_t r;
  ZeroMem(&t, sizeof(dev_conf_t));
  r = storage.getConfiguration(_relay);
  t.hour = r.hour;
  t.min = r.min;
  t.duration = (r.duration / SECONDS_IN_A_MINUTE); // duration saved in seconds and show in min in Blynk
  t.data |= (r.days & 0x7F);
  return t;
}

void updateScreen(dev_conf_t t)
{

  Blynk.virtualWrite(V1, t.hour);
  Blynk.virtualWrite(V2, t.min);
  Blynk.virtualWrite(V3, t.duration);
  Blynk.virtualWrite(V5, t.monday);  // if Monday is on, show all odd days on, otherwise all off
  Blynk.virtualWrite(V6, t.tuesday); // if Tuesday is on show all even days on, otherwise all off
}

// Send the current values to the app
// send the values for relay1, relay are mutal exclusive
BLYNK_CONNECTED()
{
  // set relay 1 enable
  Blynk.virtualWrite(V0,1);
  // Update the screen with values for relay 1
  temp_inf = getConfFromMem(RELAY1);
  updateScreen(temp_inf);
}

BLYNK_WRITE_DEFAULT()
{
  uint8_t pin = request.pin;
  switch (pin)
  {
  case V4: // Save
    // save the values to the corresponding relay
    Blynk.syncAll();
    storage.saveConfiguration(temp_relay, temp_inf.hour, temp_inf.min, 0, temp_inf.duration, (temp_inf.data & 0x7F));
    logger << LOG_INFO << "Configuration for relay" << temp_relay << " = " << temp_inf.data << EndLine;
    Blynk.virtualWrite(V4, false);
    storage.dumpEEPROMValues();
    break;
  default:
    break;
  }
}

// update data according the slide position
BLYNK_WRITE(V0)
{
  // 0 = RELAY1, ...
  temp_relay = param.asInt() - 1; // use 1 to 4 in the GUI
  // update the screen with current values
  temp_inf = getConfFromMem(temp_relay);
  updateScreen(temp_inf);
  logger << LOG_INFO << "loading conf for relay" << temp_relay  << " " << temp_inf.data << EndLine;
}

// Odd days: Monday, Wednesday, Friday, Sunday
BLYNK_WRITE(V5)
{
  if (param.asInt()) {
    temp_inf.data |= 0x2B;
  }
  else {
    temp_inf.data &= 0xFFFFFFD4;
  }
}

// Even days: Tuesday, Thursday, Saturday
BLYNK_WRITE(V6)
{
  if (param.asInt()) {
    temp_inf.data |= 0x54;
  }
  else {
    temp_inf.data &= 0xFFFFFFAB;
  }
}

BLYNK_WRITE(V1)
{
  temp_inf.hour = param.asInt();
}

BLYNK_WRITE(V2)
{
  temp_inf.min = param.asInt();
}

BLYNK_WRITE(V3)
{
  temp_inf.duration = (param.asInt() * SECONDS_IN_A_MINUTE); // to convert to seconds
}
