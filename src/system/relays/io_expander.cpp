#include <Arduino.h>
#include "io_expander.h"
#include <stdint.h>
#include "utils/logger.h"

bool IOExpander::init(uint8_t address)
{
    logger << LOG_DEBUG << "Initializing I2C IOExpander with address 0x" << INT_HEX << address << EndLine;
    i2cDevice.setAddress(address);

    bool success = i2cDevice.isConnected();
    if (!success) {
        logger << LOG_ERROR << "Can't initialize RTC device..." << EndLine;
    }
    return success;
}

bool IOExpander::write(IOActionPin_e position, bool state)
{
    uint8_t currOutputSt, newOutputSt, mask, error_N;
    // Reading current value out all output register (8 bits)


    currOutputSt = i2cDevice.readByte(PCA9554_OUTPUT_REG1);     
    Serial.print(currOutputSt, BIN);
    Serial.print("\n\r");

    // Mask specific bit to be changed
    if (state)
    {
        mask = 1 << position;
        newOutputSt = currOutputSt | mask;
    }
    else
    {
        mask = ~(1 << position);
        newOutputSt = currOutputSt & mask;
    }
    error_N = i2cDevice.writeWord(PCA9554_OUTPUT_REG1 | (newOutputSt << 8));

    return error_N;
}

bool IOExpander::read(IOActionPin_e position)
{
    uint8_t readData, maskR;

    maskR = 0x01; // Mask to ready the LSB

    readData = i2cDevice.readByte(PCA9554_OUTPUT_REG1);// Read the value of output register
    readData = readData >> position; // shift right to set LSB as interest data
    readData = readData & maskR;     // Filter the LSB

    if (readData == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}