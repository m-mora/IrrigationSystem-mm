#include "i2cMaster.h"
#include <Arduino.h>
#include <stdint.h>




void I2CMaster::init(uint8_t address)
{
    ADDR = address;
    Wire.begin(); // Configuring ESP as I2C controller
}


bool I2CMaster::i2cScan()
{
    uint8_t additer = 0;
    uint8_t error = 0;
    bool retVal = false;

    for (additer = 0; additer <= 0xff; additer++)
    {
        Wire.beginTransmission(additer);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("Device Found at = 0x");
            Serial.print(additer,HEX);
            Serial.print("\n\r");
            retVal = true;
        }
    }

    return retVal;
}



bool I2CMaster::writeByte(uint8_t data)
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

uint8_t I2CMaster::readByte(uint8_t regNum)
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

bool I2CMaster::writeWord(uint8_t byte1, uint8_t byte2)
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
