#include <stdio.h>
int getUserInput(const char* msg, int* dataAddr, int lowBound,int upBound);
 
int main(void) {
    int deviceNum = 0;
    int channelNum = 0;
    int pwmVal = 0;

    const char* m1="Please enter device number. ";
    const char* m2="Please enter channel number. ";
    const char* m3="Please enter PWM value. ";
     
    getUserInput(m1, &deviceNum, 1, 7);
    getUserInput(m2, &channelNum, 1, 16);
    getUserInput(m3, &pwmVal, 1, 100);


    printf("Please confirm the selection:\n"
            "device number: %d \n"
            "channel number: %d \n"
            "PWM value: %d \n ",
            deviceNum, channelNum, pwmVal);
}


int getUserInput(   const char* msg, // string message UI
                    int* dataAddr,   // address to store user data input
                    int lowBound,
                    int upBound
        ){

    printf("%s", msg);
    printf("The value should range between %d and %d: ", lowBound, upBound);

    scanf("%d", dataAddr);
    return 0;
}


