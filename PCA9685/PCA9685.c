#include "PCA9685.h"

int PCA_configDevice(PCA* pointerPCA, mcp2221_t** mcp_dev){

    // Get 8-bit device address from LUT and register address into struct
    PCA_regDeviceAddr(pointerPCA);
    PCA_regI2cDriver(pointerPCA, mcpdev);
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
        pointerPCA->address = addTable[deviceNum-1];
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
    pointerPCA-> pwmVal[16]={0};
    pointerPCA-> onCount[16]={0};
    pointerPCA-> offCount[16]={0};
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
        case (SINGLE):
            PCA_writePWM_2channel(pointerPCA, pointerPCA->channelNum, pointerPCA->PWM);
        case (BATCH):
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
    return 0;
}

int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int PWM){
    /* Write current PWM to current channel 
     */
    
    // holder for ther 4 reg list
    uint8_t LED_reg_list[4];
    // From LUT, lookup channel register address base on channelNum
    getChannelReg(channelNum, LED_reg_list); 

    // Calculate individual the individual register values
    // TODO: check datasheet of how this is implemented (how bits are splitted btwn two regs)
    //       certain bits in the registers are not writable, do we avoid this?
    /*TODO: figure out how to tell this scope the pwm counter values
     * maybe call PCA_calcPWM here? since this is not impoerant to the main level
     *uint8_t LED_ON_LOW_VAL = 0x00;
     *uint8_t LED_ON_LOW_VAL = 0x00;
     *uint8_t LED_ON_LOW_VAL = 0x00;
     *uint8_t LED_ON_LOW_VAL = 0x00;
     */


    // Calls PCA_writeReg for each of (LED_ON_LOW, LED_ON_HIGH, LED_OFF_LOW, LED_OFF_HIGH)

    PCA_writeReg(LED_ON_LOW_REG, LED_ON_LOW_VAL);
    PCA_writeReg(LED_ON_HIGH_REG, LED_ON_HIGH_VAL);
    PCA_writeReg(LED_OFF_LOW_REG, LED_OFF_LOW_VAL);
    PCA_writeReg(LED_OFF_HIGH_REG, LED_OFF_HIGH_VAL);

    return 0;
}

int PCA_writeReg(uint8_t* regAddr, uint8_t regData){
    // Call MCP driver to write data via I2C protocol
}

int PCA_calcPWM(PCA* pointerPCA){
    /* Move PWM control code here
     * (calculate on and off counter values)
     */
    int delay = 0;
    int width = 4096;
    int* onCounterAddr = (pointerPCA->onCount)+ sizeof(int)*(pointerPCA->channelNum-1);
    int* offCounterAddr = (pointerPCA->offCount)+ sizeof(int)*(pointerPCA->channelNum-1);

    calcPWM((pointerPCA->pwmVal), delay, width, onCounterAddr, offCounterAddr);
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
        return 0;
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
}
