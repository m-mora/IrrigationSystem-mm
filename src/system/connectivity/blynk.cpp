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
static uint8 temp_relay;

void KlicBlynk::init(const char *_auth) {
  Blynk.config(_auth);
}

dev_conf_t getConfFromMem(uint8 _relay) {
  uint8_t days = 0;
  dev_conf_t t;
  ZeroMem(&t,sizeof(dev_conf_t));
  storage.getConfiguration(_relay, t.hora, t.min, t._, t.duration, days);
  t.data |= (days & 0x7F);

  return t;
}

void updateScreen(dev_conf_t t) {

  Blynk.virtualWrite(V0, t.lunes);
  Blynk.virtualWrite(V1, t.martes);
  Blynk.virtualWrite(V2, t.miercoles);
  Blynk.virtualWrite(V3, t.jueves);
  Blynk.virtualWrite(V4, t.viernes);
  Blynk.virtualWrite(V5, t.sabado);
  Blynk.virtualWrite(V6, t.domingo);
  Blynk.virtualWrite(V7, t.hora);
  Blynk.virtualWrite(V8, t.min);
  Blynk.virtualWrite(V9, t.duration);
}

// Send the current values to the app
// send the values for relay1, relay are mutal exclusive
BLYNK_CONNECTED() {
  Blynk.virtualWrite(V10, true);
  Blynk.virtualWrite(V11, false);
  Blynk.virtualWrite(V12, false);
  Blynk.virtualWrite(V13, false);
  // Update the screen with values for relay 1
  temp_inf = getConfFromMem(RELAY1);
  updateScreen(temp_inf);
}

BLYNK_WRITE_DEFAULT() {
  uint8_t pin = request.pin;
  switch (pin) {
    case V18:
      // "SAVE" pin has been pressed, save the values to the
      // corresponding relay
      Blynk.syncAll();
      storage.saveConfiguration(temp_relay, temp_inf.hora, temp_inf.min, 0, temp_inf.duration, (temp_inf.data & 0x7F));
      logger << LOG_INFO << "Configuration for relay1(0) " << temp_inf.data << EndLine;
      Blynk.virtualWrite(V18, false);
      break;
    default:
      break;
  }
}

// Select relay1, disable 2,3,4
BLYNK_WRITE(V10) {
  Blynk.virtualWrite(V11, false);
  Blynk.virtualWrite(V12, false);
  Blynk.virtualWrite(V13, false);
  temp_relay = RELAY1;
  // update the screen with current values
  temp_inf = getConfFromMem(temp_relay);
  updateScreen(temp_inf);
  logger << LOG_INFO << "loading conf for relay1(0) " << temp_inf.data << EndLine;
}

// Select relay2, disable 1,3,4
BLYNK_WRITE(V11) {
  Blynk.virtualWrite(V10, false);
  Blynk.virtualWrite(V12, false);
  Blynk.virtualWrite(V13, false);
  temp_relay = RELAY2;
  // update the screen with current values
  temp_inf = getConfFromMem(temp_relay);
  updateScreen(temp_inf);
  logger << LOG_INFO << "loading conf for relay2 " << temp_inf.data << EndLine;
}

// Select relay3, disable 1,2,4
BLYNK_WRITE(V12) {
  Blynk.virtualWrite(V10, false);
  Blynk.virtualWrite(V11, false);
  Blynk.virtualWrite(V13, false);
  temp_relay = RELAY3;
  // update the screen with current values
  temp_inf = getConfFromMem(temp_relay);
  updateScreen(temp_inf);
  logger << LOG_INFO << "loading conf for relay3 " << temp_inf.data << EndLine;
}

// Select relay4, disable 1,2,3
BLYNK_WRITE(V13) {
  Blynk.virtualWrite(V10, false);
  Blynk.virtualWrite(V11, false);
  Blynk.virtualWrite(V12, false);
  temp_relay = RELAY4;
  // update the screen with current values
  temp_inf = getConfFromMem(temp_relay);
  updateScreen(temp_inf);
  logger << LOG_INFO << "loading conf for relay4 " << temp_inf.data << EndLine;
}

BLYNK_WRITE(V0) {
  temp_inf.lunes = param.asInt();
}

BLYNK_WRITE(V1) {
  temp_inf.martes = param.asInt();
}

BLYNK_WRITE(V2) {
  temp_inf.miercoles = param.asInt();
}

BLYNK_WRITE(V3) {
  temp_inf.jueves = param.asInt();
}

BLYNK_WRITE(V4) {
  temp_inf.viernes = param.asInt();
}
BLYNK_WRITE(V5) {
  temp_inf.sabado = param.asInt();
}
BLYNK_WRITE(V6) {
  temp_inf.domingo = param.asInt();
}

BLYNK_WRITE(V7) {
  temp_inf.hora = param.asInt();
}

BLYNK_WRITE(V8) {
  temp_inf.min = param.asInt();
}

BLYNK_WRITE(V9) {
  temp_inf.duration = param.asInt();
}
