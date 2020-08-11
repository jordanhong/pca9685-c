#
pca9685-c
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

July 10

- Re-structured code such that now we have 4 core master functions:
    1. `int PCA_master_init(int numDevices);` \
        Called at beginning, return array of (numDevices) PCA structs
    2. `int PCA_master_writePWM(writePWM_state mode);` \
        Asks for input and set specific device/channel/PWM \
        Two modes:   BATCH  (writes to all registers according to array)\
                     SINGLE (writes to specific device/channels without changing existing modes)
    3. `int PCA_master_restart();` \
        Reset all values to 0.
    4. `int PCA_master_getState();`  \
        Prints out all active channels information from each device. 

  
- Divided `PCA9685.c`: functions are now in categories:
    1. Master functions (`PCA_master_*`): master functions called by the top level UI. Usually passed in array of 7 drivers (PCA structs), has control over 7 drivers.
    2. PCA driver functions (`PCA_*`): driver-functions, only has knowledge of single driver. 
    3. I2C and register level functions: takes care of I2C, writing to registers, and proxy for writing to registers in the absence of driver hardware to test with. Users should change the functions here should a different I2C driver be used.  
    4. Auxiliary functions: Miscellaneous functions. 

- **TODO**:
    - More test cases, esp. edge cases
    - error checking/ guarding
    - PWM with different delay scenarios
 
July 13

- Compiled and tested Top level functions with valgrind. Code is working
- **TODO**:
    - More test cases, esp. edge cases
    - error checking/ guarding
    - PWM with different delay scenarios

July 14
- Implemented error checking for top master functions. Tested. 
- Program now asks if user is finished configuring after each input. Allows for multi-channel configuration in SINGLE mode.
- **TODO**
    - Edge cases
    - Different PWM delay
    - Check the remaining three function groups. 
    - Complete documentation. 

July 15
- Revised and fixed MCP I2C writing. Matched proxy I2C write arguments with real i2c functions. 
- PWM convert: Added support for cases when off occurs before on in a period. 
- Current implementation sets delay to be 10% in-function. 
- Tested output commands in proxy, confirmed same output as per the PCA9685 datasheet (page 16, LED Output and PWM control)
- Tested writing to edge devices and channels (device 1 and 7, channel 1 and 16)
- **TODO**
    - Check whether need to add support to ask for delay, or is delay always 0. (Would this create a RS latch metastable effect? check)
    - Current restart is implemented by manually writing 0s. Confirm whether this works, and necessity to use designated pins for global control. 
    - (Potential) Support configuration of different delays from UI. 

July 20 
- Added define statement for LUT (PCA programmable address)
- TODO: Add PCA_full_ON (100%) case

July 21
- Added FULL on case and  verified with proxy

July 22
- Started Doxygen comment in PCA9685.h

Aug 11
- Completed PCA9685 comments
- Added read from file for address table. 
- **TODO**
    - Documentation for io
    - Parametrize file names

