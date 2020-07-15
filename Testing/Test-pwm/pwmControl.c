#include <stdio.h>
#include "pwmControl.h"


// Main program to run PWM drivers


int main(void){
    int width = 4096;
    //int PWM = 20;
    //int delay = 10;
    int PWM = 90;
    int delay = 90;

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
    // printf("on time un-rounded: %f\n", onTime);
    //printf("delay time rounded: %d\n", roundNo(delayTime));
    //printf("on time rounded: %d\n", roundNo(onTime));
    //printf("off time rounded: %d\n", roundNo(onTime)+roundNo(delayTime));
    
    // Assign count values
    // note: counter starts at 0, so subtract 1 from counter values
    int onVal  = roundNo(delayTime) - 1;
    int offVal = roundNo(delayTime) + roundNo(onTime) - 1;
    //printf("offVal rounded: %d\n", offVal);

    // Case where (delayTime+PWM) >100%
    // Subtract width (next cycle)
    if (offVal>width) offVal-= width;

    *onCount  = onVal;
    *offCount = offVal;

    return 0;
}


 int roundNo(float num){ 
    return (int)(num < 0 ? num - 0.5 : num + 0.5); 
} 

  
    


