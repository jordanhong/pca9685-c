#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "PCA9685.h"
#include "io.c"
#include "MCP2221A.c"

int PCA_regI2cDriver(PCA* pointerPCA, mcp2221_t** mcp_dev);

int PCA_configDevice(PCA* pointerPCA, mcp2221_t** mcp_dev){

    // Get 8-bit device address from LUT and register address into struct
    PCA_regDeviceAddr(pointerPCA);
    PCA_regI2cDriver(pointerPCA, mcp_dev);
    return 0;
}
int PCA_regDeviceAddr(PCA* pointerPCA){
    /* Device Address Format
     * [A6  A5  A3  A2  A1  A0]
     * 
     * Constraints (from datasheet)
     *  1. A6 is fixed at 1
     *  2. 1110 000 --> LED all call
     *  3. 0000 0110--> Software reset
     */
    /* Example addresses, to be changed:
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

    uint8_t addTable [7] ={ 0x40, 0x41, 0x42, 0x44, 0x48, 0x50, 0x60};

    if ( ( (pointerPCA->deviceNum) <lowBound) || ( (pointerPCA->deviceNum) >upBound) ){
        return -1;
    }
    else{
        pointerPCA->address = addTable[(pointerPCA->deviceNum)-1];
        return 0;
    }
}
int PCA_regI2cDriver(PCA* pointerPCA, mcp2221_t** mcp_dev){
    pointerPCA->mcp_dev = mcp_dev;
    return 0;
}

int PCA_init(PCA* pointerPCA){

    // allocate memory
    pointerPCA = (PCA*)malloc(sizeof(PCA));
    PCA_reset(pointerPCA);  

    const char* m0="Please enter device number. ";
    const char* m1="Please enter channel number. ";
    const char* m2="Please enter PWM value. ";
     
    getUserInput(m0, &(pointerPCA->deviceNum) , 1, 7);
    getUserInput(m1, &(pointerPCA->current_channelNum), 1, 16);
    getUserInput(m2, &(pointerPCA->current_pwmVal), 1, 100);


    printf("Please confirm the selection:\n"
            "device number: %d \n"
            "channel number: %d \n"
            "PWM value: %d \n ",
           pointerPCA->deviceNum, pointerPCA->current_channelNum, pointerPCA->current_pwmVal);

    return 0;
}

int PCA_reset(PCA* pointerPCA){
    pointerPCA-> deviceNum = 0;
    pointerPCA-> address = 0x00;
    memset( (pointerPCA->offCount), 0, sizeof(pointerPCA->offCount) );
    memset( (pointerPCA->onCount), 0, sizeof(pointerPCA->onCount) );
    memset( (pointerPCA->pwmVal), 0, sizeof(pointerPCA->pwmVal) );
    pointerPCA-> current_channelNum=0; 
    pointerPCA-> current_pwmVal=0;       
    pointerPCA-> mcp_dev=0;

    return 0;
}

int PCA_writePWM(PCA* pointerPCA, writePWM_state mode){
    /*
     * MODE SINGLE: write "current_pwmVal" to "current_channelNum", requires updated current_channelNum and current_pwmVal.
     * MODE BATCH: check through all channels and write specific PWM values to channels.
     */
    switch(mode){
        case (SINGLE):{
            PCA_writePWM_2channel(pointerPCA, pointerPCA->current_channelNum, pointerPCA->current_pwmVal);
                      }
        case (BATCH):{
            int i = 0;
            int pwm = 0;
            for (i=0;i<16;i++){
                // check through all 16 channels in the chip
                // if PWM is non-zero,write to channel
                pwm = (pointerPCA->pwmVal)[i];
                if (pwm!= 0){
                    PCA_writePWM_2channel(pointerPCA, (i+1), pwm);
                }
            }
                     }
    }
    return 0;
}

int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int pwm){
    /* Write current PWM to current channel 
     */
    
    // holder for ther 4 reg list
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
    getChannelReg(channelNum, LED_reg_list); 

    // Assign register address
    LED_ON_LOW_REG      = LED_reg_list[0];
    LED_ON_HIGH_REG     = LED_reg_list[1];     
    LED_OFF_HIGH_REG    = LED_reg_list[2];    
    LED_OFF_LOW_REG     = LED_reg_list[3];   


    // Calculate individual the individual counter values
    // Updates array in struct (onCount, offCount)
    PCA_calcPWM(pointerPCA, channelNum, pwm);
 
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

int PCA_writeReg(PCA* pointerPCA, uint8_t regAddr, uint8_t regData){
    // TODO: Include proxy functions to test in absence of hardware
    MCP_i2cWrite(pointerPCA, regAddr, regData); 
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
    int* onCounterAddr = (pointerPCA->onCount)+ sizeof(int)*(channelNum-1);
    int* offCounterAddr = (pointerPCA->offCount)+ sizeof(int)*(channelNum-1);

    calcPWM(pwmVal, delay, width, onCounterAddr, offCounterAddr);
    return 0;
}

int calcPWM( int PWM, int delay, int width, int* onCount, int* offCount){
//int calcPWM(    // Input 
//                int PWM,    // PWM duty cycle in %
//                int delay,  // Delay time in %
//                int width,  // Total width (i.e. 4096 for 12-bit PWM
//               
//                // Output 
//                int* onCount,   // Counter value for setting ON
//                int* offCount  // Counter value for setting OFF
//                ){
    
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
    *onCount = roundNo(delayTime) - 1;
    *offCount = roundNo(delayTime + onTime - 1);

    return 0;
}

int getChannelReg(int chNum, uint8_t* regList){
    /*
     * Paramater    Type        Function                        Range
     * ==============================================================
     * chNum        int         channel number                  1-16 
     * regList      uint8_t*    array to store 4 reg # (8 bit)  4 (size) 
     *
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

int roundNo(float num){ 
    return (int)(num < 0 ? num - 0.5 : num + 0.5); 
} 
