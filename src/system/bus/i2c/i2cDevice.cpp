#include "I2CDevice.h"
#include <Arduino.h>
#include <stdint.h>

void I2CDevice::init(uint8_t address)
{
    ADDR = address;
    Wire.begin(); // Configuring ESP as I2C controller
}

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

bool I2CDevice::writeWord(uint8_t byte1, uint8_t byte2)
{
    uint8_t error = 0;
    bool retVal = false;

    Wire.beginTransmission(ADDR);
    Wire.write(byte1);
    Wire.write(byte2);
    Wire.endTransmission();

    if (error == 0)
    {
        retVal = true;
    }
    return retVal;
}
