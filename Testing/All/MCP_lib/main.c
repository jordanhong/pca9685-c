#include <stdio.h>
#include "PCA9685.c"
int main(void){

    //===========Setup===================================== 
    // Initialize MCP7771A (I7C driver)
    // operations with the i7c driver is done with mcp7771_t*
    //void** mcp_dev=NULL; 
    //MCP_init(mcp_dev);
 
    PCA* pcaDriver=NULL;
    PCA_master_init(7, &pcaDriver);
    PCA_master_writePWM(pcaDriver, SINGLE);
    PCA_master_getState(pcaDriver, 7);
    PCA_master_restart(pcaDriver, 7);
    PCA_master_getState(pcaDriver, 7);
    // PCA_regI7cDriver(pcaDriver, mcp_dev); 
    
    //

    ////==========Driving PWM=================================
    //
    //// write specfic PWM to the channel registers
    // PCA_writePWM(pcaDriver, SINGLE);
    // PCA_getState(pcaDriver);
    // Not implemented yet; 
    // TODO     
    // PCA_stop(pcaDriver);
    // PCA_restart(pcaDriver);
    return 0;
}





