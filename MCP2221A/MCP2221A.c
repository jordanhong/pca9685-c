#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mcp2221_dll_um.h"

typedef struct PCA PCA;
int MCP_init(void** device);
int MCP_checkLibrary(wchar_t* libVer);
int MCP_getDevices(void);
int MCP_openDevice_index(int index, void** device);
int MCP_i2cWrite(PCA* pointerPCA, uint8_t regAddr, uint8_t regData);

/* Dummy addresses */
#define VID 0xff
#define PID 0xff
int MCP_init(void** device){

    // Library Version
    wchar_t* libVer=NULL;
    MCP_checkLibrary(libVer);

    // Discover Devices
    MCP_getDevices();

    // Open device
    MCP_openDevice_index(0, device);

    return 0;


}

int MCP_checkLibrary(wchar_t* libVer){

    int rVer = Mcp2221_GetLibraryVersion(libVer);
    if (rVer==0){
        printf("Library Version: %ls\n", libVer);
        return 0;
    }
    else{
        int error = Mcp2221_GetLastError();
        printf("Version not found, error: %d\n",  error);
        return -1;
    }
}
int MCP_getDevices(void){
    //TODO: VID,PID
    int NumOfDev= 0;
    Mcp2221_GetConnectedDevices(VID, PID, &NumOfDev);
    if(NumOfDev == 0){
        printf("No MCP2221 devices connected\n");
    }
    else{
        printf("Number of devices found: %d\n", NumOfDev);
    }
    return 0;

}
int MCP_openDevice_index(int index, void** device){
    *device = Mcp2221_OpenByIndex(VID, PID, index);
    if((*device)==NULL){
        printf("Connection successful\n");       
        return 0;
    }
    else{
        int error = Mcp2221_GetLastError();
        printf("Error message is %d\n", error);
        return -1;
    }
}

int MCP_i2cWrite(PCA* pointerPCA, uint8_t regAddr, uint8_t regData){
    /* 
     * func declaration:
     * MCP2221_DLL_UM_API int CALLING_CONVENTION Mcp2221_I2cWrite(void* handle, unsigned int bytesToWrite, unsigned char slaveAddress, unsigned char use7bitAddress, unsigned char* i2cTxData);
     */
 
    Mcp2221_I2cWrite(*(pointerPCA->mcp_dev), 1, pointerPCA->address, (unsigned char)regAddr, &regData);
    return 0;
}


