#include <stdio.h>
#include "PCA9685.c"
int main(void){

    //===========Setup===================================== 
    // Initialize MCP2221A (I2C driver)
    // operations with the i2c driver is done with mcp2221_t*
    //void** mcp_dev=NULL; 
    //MCP_init(mcp_dev);
 
    PCA* pcaDriver=NULL;
    PCA_init(&pcaDriver); /* Asks for user input: deviceNum, channelNum, PWM*/
    // PCA_regI2cDriver(pcaDriver, mcp_dev); 
    
    //

    ////==========Driving PWM=================================
    //
    //// write specfic PWM to the channel registers
    PCA_writePWM(pcaDriver, SINGLE);
    PCA_getState(pcaDriver);
    // Not implemented yet; 
    // TODO     
    // PCA_stop(pcaDriver);
    // PCA_restart(pcaDriver);
    return 0;
}





