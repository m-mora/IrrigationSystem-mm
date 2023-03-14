#include "i2cDevice.h"
#include <Arduino.h>
#include <stdint.h>

bool I2CDevice::writeByte(uint8_t data)
{
    uint8_t error = 0;
    bool retVal = false;

    Wire.beginTransmission(ADDR);
    Wire.write(data);
    error = Wire.endTransmission();

    if (error == 0)
    {
        retVal = true;
    }

    return retVal;
}

uint8_t I2CDevice::readByte(uint8_t regNum)
{
    uint8_t readData;

    Wire.beginTransmission(ADDR);
    Wire.write(regNum);
    Wire.endTransmission();

    if (Wire.requestFrom((int)ADDR, 1) != 1)
    {
        return false;
    };

    readData = Wire.read();
    return readData;
}

bool I2CDevice::writeWord(uint16_t word)
{
    uint8_t error = 0;
    bool retVal = false;

    Wire.beginTransmission(ADDR);
    Wire.write(word & 0xFF);
    Wire.write((word >> 8) & 0xFF);
    error = Wire.endTransmission();

    if (error == 0)
    {
        retVal = true;
    }
    return retVal;
}
