# pca9685-c
This is a C library to control the NXP Semiconductors PCA9685 16-channel 12-bit PWM I2C-bus LED Controller. The library includes an user interface which prompts the user to enter the device number (1-7), channel number(1-16), and specify a PWM value in percentage. 

The library uses a [MCP2221A](https://www.microchip.com/wwwproducts/en/MCP2221A) (USB 2.0 to I2C/UART Protocol Converter) for I2C communication. The [MCP2221A library](MCP2221A/lib/) is adapted and modified from [zkemble/libmcp2221](https://github.com/zkemble/libmcp2221/tree/master/libmcp2221). 

Status: Development in progress.

## Development Log

July 02

- Checked libmcp2221 i2c functions limit on writing (60 bytes max, not an issue for our purposes)
- Incorporated libmcp2221 i2c functions into self-defined MCP2221A library.
- [MCP2221A library] Completed proxy functions to substitute I2C devices. Tested writing dummy I2C packets into text files.
- [PCA9685 library] Incorporated MCP2221A functions for I2C protocols.
- [IO library] Re-structured input (UI) and output (text file output) into IO module.

- **TODO**: 
    1. Test functions from top level. Note for self: need to deal with functions reporting errors now for lack of hardware.
    2. Implement [PCA9685] functions: global restart (reset?), write to all mode, sleep mode.

July 07

- Checked libmcp2221 library: not fully implemented, poor completion and hard to test.
- **TODO**:
    1. Read MCP2221 dll User guide.
    2. Implement into MCP class

July 09

- Implemented MCP2221 dll proprietary library. (Not tested as to many errors and cannot compile)
- Integrated modules such that program can compile without errors, using proxy functions. 
- The MCP2221 dll library takes care of the I2C bus specifics, i.e. to write to a slave, one simply invoke:\
   `Mcp2221_I2cWrite(void* handle, unsigned int bytesToWrite, unsigned char slaveAddress, unsigned char use7bitAddress, unsigned char* i2cTxData)`
- Therefore, current code does not concern the various commands. Proxy I2C function writes to a text file with slave address / reg/ reg data/ ....
- **TODO**: 
    - More test cases, esp. edge cases
    - error checking/ guarding
    - PWM with different delay scenarios
    - Restart (write to all 0)
