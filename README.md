#
pca9685-c
This is a C library to control the NXP Semiconductors PCA9685 16-channel 12-bit PWM I2C-bus LED Controller. The library includes an user interface which prompts the user to enter the device number (1-7), channel number(1-16), and specify a PWM value in percentage. 

The library uses a [MCP2221A](https://www.microchip.com/wwwproducts/en/MCP2221A) (USB 2.0 to I2C/UART Protocol Converter) for I2C communication. The [MCP2221A library](MCP2221A/lib/) is adapted and modified from [zkemble/libmcp2221](https://github.com/zkemble/libmcp2221/tree/master/libmcp2221). 

Status: Completed software component. MCP2221A device not handled.


# Structure
- IO: input/ output functions
- PCA9685: majority of functions. Deals with programming the PCA9685 and writing to specific pins. Utilizes MCP2221A handles, but that part has not been tested yet. So far I have been using proxy functions (writing to text files)
- MCP2221A: low level I2C library from microchip. Was not able to test on laptop.
