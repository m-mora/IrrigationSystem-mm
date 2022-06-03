#include <Arduino.h>
#include "io_expander.h"
#include <Wire.h>
#include <stdint.h>

bool IOExpander::init(uint8_t address)
{
    uint8_t error;
    bool retuVal = false;

    ADDR = address;

    Wire.begin(); // Configuring ESP as I2C controller
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {

        Serial.print("****Configuration of IOExpander PCA9554***\n");
        Serial.print("Configurating Reg3: Configuration Register\n");
        Wire.beginTransmission(address);
        Wire.write(PCA9554_CONFIG_REG3); // Number of Configuration register
        Wire.write(0x00);                // Configurating as output all
        Wire.endTransmission();          // Stop transmitting

        // Safe State: Configuring all outputs to zero volts
        Serial.print("Configurating Reg1: Configuration Register\n");
        Wire.beginTransmission(address);
        Wire.write(PCA9554_OUTPUT_REG1); // Number of Configuration register
        Wire.write(0x00);                // Turning off all outputs
        Wire.endTransmission();          // Stop transmitting

        retuVal = true;
    }

    return retuVal;
}

bool IOExpander::write(IOActionPin_e position, bool state)
{
    uint8_t currOutputSt, newOutputSt, mask, error;
    // Reading current value out all output register (8 bits)

    Wire.beginTransmission(ADDR);
    Wire.write(PCA9554_OUTPUT_REG1); // Calling to register # 1 (Output register)
    Wire.endTransmission();          // Configuration register
    if (Wire.requestFrom((int)ADDR, 1) != 1)
    {
        return false; // error code is above normal data range
    };

    currOutputSt = Wire.read();
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

    Wire.beginTransmission(ADDR);
    Wire.write(PCA9554_OUTPUT_REG1); // Configuration register
    Wire.write(newOutputSt);         // Configuring as output all
    error = Wire.endTransmission();  // Stop transmitting

    if (error != 0)
    {
        Serial.print("ERROR\n\r");
        return false;
    }
    else
    {
        Serial.print("OK\n\r");
        return true;
    }
}

bool IOExpander::read(IOActionPin_e position)
{
    uint8_t _inputData, maskR;

    maskR = 0x01; // Mask to ready the LSB

    Wire.beginTransmission(ADDR);
    Wire.write(PCA9554_OUTPUT_REG1);
    Wire.endTransmission(); // Configuration register

    if (Wire.requestFrom((int)ADDR, 1) != 1)
    {
        return false; // error code is above normal data range
    };

    _inputData = Wire.read();            // Read the value of output register
    _inputData = _inputData >> position; // shift right to set LSB as interest data
    _inputData = _inputData & maskR;     // Filter the LSB

    if (_inputData == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}