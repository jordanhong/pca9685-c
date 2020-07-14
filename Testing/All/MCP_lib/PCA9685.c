#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "PCA9685.h"
#include "io.c"
//#include "MCP2221A.c"

#define TOTAL_PCA_NUM 7
/******************* Master functions (Global perspective)****************************/
int PCA_master_init(int numDevices, PCA** arrayHeader){
    
    // Allocate memory for an array of PCA devices
    (*arrayHeader) = (PCA*) malloc (numDevices*sizeof(PCA));
    if ((*arrayHeader)==NULL){
            printf("Maloc unsuccessful!\n");
            return -1;
    }
    // For each of the device:
    //  1. Condigure device number and label
    //  2. initialize values
    int deviceNum = 0;
    PCA* pointerPCA = NULL;
    for (deviceNum = 0; deviceNum<numDevices; deviceNum++){
        pointerPCA = (*arrayHeader) + deviceNum;
        pointerPCA->deviceNum = deviceNum;
        pointerPCA->address = PCA_getDeviceAddr(pointerPCA->deviceNum);
        PCA_reset(pointerPCA);
    }
    return 0;
}

int PCA_master_writePWM(PCA* master , writePWM_state mode){
    int deviceNum=0;
    int channelNum=0;
    int pwmVal=0;

    if (master==NULL){
        printf("Null device address,aborting.\n");
        return -1;
    }

    switch(mode){
        case(SINGLE):;
            // Write to individual channel registers
            // Continuously asks user the device-channel-pwm, until user quits

            char status = 'n'; // char to store status (Y/n);
            int  done = 0;

            const char* m0="Please enter device number. ";
            const char* m1="Please enter channel number. ";
            const char* m2="Please enter PWM value. ";
            const char* m3="Configuration complete ? [Y/n]";
            
            while (!done){
                // Ask for user input
                getUserInput(m0, &(deviceNum) , 1, 7);
                getUserInput(m1, &(channelNum), 1, 16);
                getUserInput(m2, &(pwmVal), 1, 100);
                
                // Confirmation
                printf("Please confirm the selection:\n"
                       "device number: %d \n"
                       "channel number: %d \n"
                       "PWM value: %d \n ",
                       deviceNum, channelNum, pwmVal);

                // Update struct and write registers
                PCA* pointerPCA;
                pointerPCA = master + (deviceNum-1);
                PCA_setPWM(pointerPCA, channelNum, pwmVal);
                PCA_writePWM_2channel(pointerPCA, channelNum, pwmVal);
                
                // Ask user if done
                char_getUserInput(m3, &status);
                done = (int) (status=='Y');

            }
            break;

        case(BATCH):;
            /* Assumes all struct is updated, writes to registers according to struct info */
            int i=0;
            int ch = 0;
            //for (i=0;i<TOTAL_PCA_NUM;i++){
            for (i=0;i<TOTAL_PCA_NUM;i++){
                PCA* pointerPCA = master + i;
                for (ch=0;ch<16;ch++){
                    // check through all 16 channels in the chip
                    // if PWM is non-zero,write to channel
                    pwmVal = (pointerPCA->pwmVal)[ch];
                    if (pwmVal!= 0){
                        printf("device %d, channel %d, has PWM value %d\n", i+1, ch+1, pwmVal);
                        PCA_writePWM_2channel(pointerPCA, (ch+1), pwmVal);
                    }
                }
            }
            break;

        default:
            printf("Invalid mode. Writing mode must be either SINGLE or BATCH. Aborting.\n");
            return -1;
    }
    return 0;

}

int PCA_master_restart(PCA* master, int numDevices){
    int deviceNum = 0;
    for (deviceNum=0;deviceNum<numDevices;deviceNum++){
        PCA* pointerPCA = master + deviceNum;
        PCA_reset(pointerPCA);
    }
    PCA_master_writePWM(master, BATCH);
    return 0;
}

int PCA_master_getState(PCA* headPCA, int numDevices){
    int i = 0;
    for (i=0;i<numDevices;i++){
        PCA* pointerPCA= headPCA + i;
        PCA_getState(pointerPCA);
        printf("==============================\n");
    }
    return 0;
}
 
/******************** PCA Driver functions********************************************/
int PCA_reset(PCA* pointerPCA){
    zero( (pointerPCA->offCount), 16);
    zero( (pointerPCA->onCount), 16);
    zero( (pointerPCA->pwmVal), 16 );
    pointerPCA-> mcp_dev= NULL;

    return 0;
}

int PCA_getState(PCA*pointerPCA){
    if (pointerPCA==NULL){
        printf("Null pointer !\n");
        return -1;
    }
    printf("Device No: %d. Device address: 0x%x. \n", (pointerPCA->deviceNum+1), pointerPCA->address);
    printf("Active channel  |  PWM  | onCount | offCount\n");
    int i = 0;
    for (i=0;i<16;i++){
        if ((pointerPCA->pwmVal)[i]!=0){
    printf("    %d          |  %d   |  %d      |   %d\n", i, pointerPCA->pwmVal[i], pointerPCA->onCount[i], pointerPCA->offCount[i]);
        }
    }
    return 0;
}

int PCA_setPWM(PCA* pointerPCA, int channel, int pwm){
    /*
     * Sets the relevant array values in the PCA struct. 
     * Calculates values for counters etc.
     * Does not write to the device registers yet
     */
    
    // Update pwmVal array value
    (pointerPCA->pwmVal)[channel-1] = pwm;
    PCA_calcPWM(pointerPCA, channel, pwm);
    return 0;
}

int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int pwm){
    /* Write current PWM to current channel 
     */
    
    // holder for the 4 reg list
    uint8_t LED_reg_list[4];

    // holder for register addresses and data (all byte-size)
    uint8_t LED_ON_LOW_REG = 0x00;
    uint8_t LED_ON_HIGH_REG = 0x00;
    uint8_t LED_OFF_HIGH_REG = 0x00;
    uint8_t LED_OFF_LOW_REG = 0x00;

    uint8_t LED_ON_HIGH_VAL = 0x00;
    uint8_t LED_ON_LOW_VAL = 0x00;
    uint8_t LED_OFF_LOW_VAL = 0x00;
    uint8_t LED_OFF_HIGH_VAL = 0x00;

     
    // From LUT, lookup channel register address base on channelNum
    PCA_getChannelReg(channelNum, LED_reg_list); 

    // Assign register address
    LED_ON_LOW_REG      = LED_reg_list[0];
    LED_ON_HIGH_REG     = LED_reg_list[1];     
    LED_OFF_HIGH_REG    = LED_reg_list[2];    
    LED_OFF_LOW_REG     = LED_reg_list[3];   



    // Assign register values
    /* According to the datasheet, each register is byte-sized. 
     * However, not all bits are writable, i.e LED_ON_H[7:5] is reserved
     * Here we pass in zeros for the reserved bits, but the zeros will not be written in.
     */
    LED_ON_LOW_VAL      = (uint8_t)((pointerPCA->onCount)[channelNum-1] & 0xff); // Mask out lower 8 bits
    LED_ON_HIGH_VAL     = (uint8_t)(( (pointerPCA->onCount)[channelNum-1] >> 8) & 0x0f); // Shift bit 9-12 to lower 4 bits and mask out
    LED_OFF_LOW_VAL      = (uint8_t)((pointerPCA->offCount)[channelNum-1] & 0xff); // Mask out lower 8 bits
    LED_OFF_HIGH_VAL     = (uint8_t)(( (pointerPCA->offCount)[channelNum-1] >> 8) & 0x0f); // Shift bit 9-12 to lower 4 bits and mask out


    // Calls PCA_writeReg for each of (LED_ON_LOW, LED_ON_HIGH, LED_OFF_LOW, LED_OFF_HIGH)

    PCA_writeReg(pointerPCA, LED_ON_LOW_REG, LED_ON_LOW_VAL);
    PCA_writeReg(pointerPCA, LED_ON_HIGH_REG, LED_ON_HIGH_VAL);
    PCA_writeReg(pointerPCA, LED_OFF_LOW_REG, LED_OFF_LOW_VAL);
    PCA_writeReg(pointerPCA, LED_OFF_HIGH_REG, LED_OFF_HIGH_VAL);

    return 0;
}

int PCA_calcPWM(PCA* pointerPCA, int channelNum, int pwmVal){
    /* Move PWM control code here
     * (calculate on and off counter values)
     */
    int delay = 0;
    int width = 4096;
    // The struct contains 2 16-int array (one for onCount and the other for offcount)
    // Calculate address of specific index based on channel number and size of integers
    int* onCounterAddr = (pointerPCA->onCount)+ channelNum-1;
    int* offCounterAddr = (pointerPCA->offCount)+ channelNum-1;

    calcPWM(pwmVal, delay, width, onCounterAddr, offCounterAddr);
    return 0;
}

/********************I2C and Register level Functions****************************************************/
int PCA_regI2cDriver(PCA* pointerPCA, void** mcp_dev){
    pointerPCA->mcp_dev = mcp_dev;
    return 0;
}

int PCA_writeReg(PCA* pointerPCA, uint8_t regAddr, uint8_t regData){

    /* Usage: MCP2221_DLL_UM_API int CALLING_CONVENTION Mcp2221_I2cWrite(void* handle, unsigned int bytesToWrite, unsigned char slaveAddress, unsigned char use7bitAddress, unsigned char* i2cTxData);*/
    MCP_i2cWrite_proxy(pointerPCA, regAddr, regData);
    //MCP_i2cWrite(pointerPCA, regAddr, regData); 
    return 0;
}

int MCP_i2cWrite_proxy(PCA* pointerPCA, uint8_t regAddr,uint8_t regData){
    uint8_t report[5];    
    //report[0] = (uint8_t) *(pointerPCA->mcp_dev); // driver handle
    report[0] = 0x00; // driver handle
    report[1] = 1;                   // bytes to write (const 1)
    report[2] = pointerPCA->address; // slave address
    report[3] = regAddr;             // register address
    report[4] = regData;             // register data

    const char* path = "proxy_MCP.txt";
    writeCommand_2file(path, report, 5);
    return 0;
}

/********************* Auxiliary functions*********************************************/
uint8_t PCA_getDeviceAddr(int deviceNum){
    /* 
     * Device Address Format
     * [A6  A5  A3  A2  A1  A0]
     * 
     * Constraints (from datasheet)
     *  1. A6 is fixed at 1
     *  2. 1110 000 --> LED all call
     *  3. 0000 0110--> Software reset
     */


    /* 
     * Example addresses, to be changed:
     * Device num   Binary      Hex
     * 1            100 0000    0x40
     * 2            100 0001    0x41
     * 3            100 0010    0x42
     * 4            100 0100    0x44
     * 5            100 1000    0x48
     * 6            101 0000    0x50
     * 7            110 0000    0x60
     */ 
    int lowBound    = 1;
    int upBound     = 7;

    uint8_t addTable [7] ={0x40, 0x41, 0x42, 0x44, 0x48, 0x50, 0x60};

    if ( (deviceNum<lowBound) || (deviceNum>upBound) ) return 0x00;
    else return addTable[deviceNum-1];
    
}

int PCA_getChannelReg(int chNum, uint8_t* regList){
    /*
     * Paramater    Type        Function                        Range
     * ==============================================================
     * chNum        int         channel number                  1-16 
     */
    
    // Function writes out the 4 register # to regList 
    // in the order of:     LED#_ON_L, LED#_OFF_H, LED#_ON_L, LED#_OFF_H 
    // Each register # represents a 12 bit register
    int lowBound    = 1;
    int upBound     = 16;
    
    /*LED   startReg    endReg
     *
     * 0    6           9
     * 1    10  
     * 2    14
     * |
     * 15   66
     * n    n*4+6       n*4+9
     */
    
    if ( (chNum<lowBound) || (chNum>upBound)){
        // printf("%s", "Channel number not in range\n");
        return -1;
    }

    else{
        // Get starting register value
        // Convert to unsigned byte
        int i = 0;
        int len = 4;
        int reg = 0;

        reg = (chNum-1)*LED_multiplier + LED_0_start;
        printf("Starting reg: %d\n", reg);
       
        for (i=0;i<len;i++){
            // cask int to unsigned byte
            uint8_t regByte = 0x00;
            // printf("Writing array: %d\n", reg);
            regByte = (uint8_t) (reg & 0xff);
            regList[i] = regByte;
            reg++;
        }
    }
    return 0;
}

int calcPWM( int PWM, int delay, int width, int* onCount, int* offCount){
    float delayTime = 0.0;  // Holds how long before ON should start
    float onTime = 0.0;     // Holds the duration of ON time

    // Calculate delay time and on time
    delayTime = (delay*width)/ (float) 100;
    onTime = (PWM*width)/ (float) 100;
    

    //printf("delay time un-rounded: %f\n", delayTime);
    //printf("on time un-rounded: %f\n", onTime);
    //printf("delay time rounded: %d\n", roundNo(delayTime));
    //printf("on time rounded: %d\n", roundNo(onTime));
    
    // Assign count values
    // note: counter starts at 0, so subtract 1 from counter values
    int onCounterVal= ((roundNo(delayTime) - 1) >0)? (roundNo(delayTime) - 1):0;
    int offCounterVal = roundNo(delayTime + onTime - 1);
    printf("Calculated onCount is %d and offCount is %d\n", roundNo(delayTime) - 1, roundNo(delayTime + onTime - 1));
    *onCount = onCounterVal; 
    *offCount = offCounterVal; 
    return 0;
}

int roundNo(float num){ 
    return (int)(num < 0 ? num - 0.5 : num + 0.5); 
} 

int zero(int* array, int size){
    if (array==NULL) {
        return -1;
    }
    int i=0;
    for (i=0;i<size;i++){
        array[i]=0;
    }
    return 0;
}
