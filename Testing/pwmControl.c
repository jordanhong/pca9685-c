#include <stdio.h>
#include "pwmControl.h"


// Main program to run PWM drivers


int main(void){
    int PWM = 20;
    int delay = 10;
    int width = 4096;

    int onCount;
    int offCount;

    calcPWM(PWM, delay, width, &onCount, &offCount);

    printf("Calculated onCount: %d\n", onCount);
    printf("Calculated offCount: %d\n", offCount);
    return 0;
}

int calcPWM(    // Input 
                int PWM,    // PWM duty cycle in %
                int delay,  // Delay time in %
                int width,  // Total width (i.e. 4096 for 12-bit PWM
               
                // Output 
                int* onCount,   // Counter value for setting ON
                int* offCount  // Counter value for setting OFF
                ){
    
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

int writePWM(   uint8_t* 4_ledAddr, // array of the 4 array addresses (LED_ON_H, LED_ON_L,LED_OFF_H, LED_OFF_L)
                uint8_t* 4_ledData  // array of the 4 values to be written 
                ){
    
    int i= 0;
    for (i=0;i<4;i++){
        writeReg(4_ledAddr[i], 4_ledData[i]);
    }
    return 0;
}


   
    
int roundNo(float num){ 
    return (int)(num < 0 ? num - 0.5 : num + 0.5); 
} 


