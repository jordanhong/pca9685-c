#include <stdio.h>
#include "MCP2221A.h"
#include "libmcp2221.c"


int MCP_init(mcp2221_t** myDev){

	puts("Starting!");

	mcp2221_init();
	
	// Get list of MCP2221s
	printf("Looking for devices... ");
	int count = mcp2221_find(MCP2221_DEFAULT_VID, MCP2221_DEFAULT_PID, NULL, NULL, NULL);
	printf("found %d devices\n", count);

	// Open whatever device was found first
	printf("Opening device... ");
	// mcp2221_t* myDev = mcp2221_open(); //here myDev is of type mcp2221_t*
	*myDev = mcp2221_open(); // here myDev is of type mcp2221_t**

	if(!(*myDev))
	{
		mcp2221_exit();
		puts("No MCP2221s found");
		getchar();
		return -1;
	}
	puts("done");
    return 0;

	
}

int MCP_i2cWrite(PCA* pointerPCA, uint8_t regAddr, uint8_t regData){
    MCP_i2cWrite_proxy(*(pointerPCA->mcp_dev), regAddr, &regData, 1, MCP2221_I2CRW_NORMAL);
    /*cask regAddr back to int to match external library*/
    mcp2221_i2cWrite(*(pointerPCA->mcp_dev), (int)regAddr, &regData, 1, MCP2221_I2CRW_NORMAL);
    return 0;
}

int MCP_i2cWrite_proxy(mcp2221_t* device, uint8_t address, void* data, int numByte, mcp2221_i2crw_t type){
    // TODO: Have to ensure that device 1=NULL when testing
    
    const char* path = "proxy_MCP.txt";

    // Copied from mcp2221_i2cWrite 
	usb_cmd_t cmd;
	switch(type)
	{
		case MCP2221_I2CRW_NORMAL:
			cmd = USB_CMD_I2CWRITE;
			break;
		case MCP2221_I2CRW_REPEATED:
			cmd = USB_CMD_I2CWRITE_REPEATSTART;
			break;
		case MCP2221_I2CRW_NOSTOP:
			cmd = USB_CMD_I2CWRITE_NOSTOP;
			break;
		default:
			cmd = USB_CMD_I2CWRITE;
			break;
	}
	NEW_REPORT(report);
    // report is array of bytes, and has format of:
    // command mode (0x90) for I2C write , LOW byte of data lenght, HIGH byte length, address, data
    // In our use case with PCA9685, data len always = 1 (byte), hence report will only be of length 5*sizeof(uint8_t)
    int len = numByte;
	mcp2221_error res;
	if((res = setReport(device, report, cmd)) != MCP2221_SUCCESS)
		return res;
	report[1] = len;
	report[2] = len>>8;
	report[3] = address;
	memcpy(&report[4], data, len);
	
    writeCommand_2file(path, report, len);
    return 0;
}


