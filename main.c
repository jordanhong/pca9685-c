#include <stdio.h>
#include "PCA9685.c"
int main(void){

    //===========Setup===================================== 
    // Initialize MCP2221A (I2C driver)
    // operations with the i2c driver is done with mcp2221_t*
    mcp2221_t** mcp_dev=NULL; 
    MCP_init(mcp_dev);
 
    // Get user input
    /* deviceNum, channelNum, pwmVal */
    PCA* pcaDriver=NULL;
    PCA_init(pcaDriver); /* Asks for user input: deviceNum, channelNum, PWM*/
    PCA_configDevice(pcaDriver, mcp_dev);

    

    //==========Driving PWM=================================
    
    // write specfic PWM to the channel registers
    PCA_writePWM(pcaDriver, BATCH);

    // Not implemented yet; 
    // TODO Check with John about specs (requirements for restart)
    // PCA_stop(pcaDriver);
    // PCA_restart(pcaDriver);
    //
    return 0;
}





