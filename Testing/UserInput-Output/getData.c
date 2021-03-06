#include <stdio.h>
int getUserInput(const char* msg, int* dataAddr, int lowBound,int upBound);
int char_getUserInput(const char* msg, char* dataAddr);
 
int main(void) {
    int deviceNum = 0;
    int channelNum = 0;
    int pwmVal = 0;
    char d='c';
    //const char* m1="Please enter device number. ";
    //const char* m2="Please enter channel number. ";
    //const char* m3="Please enter PWM value. ";
    // 
    //getUserInput(m1, &deviceNum, 1, 7);
    //getUserInput(m2, &channelNum, 1, 16);
    //getUserInput(m3, &pwmVal, 1, 100);
    char_getUserInput ("Enter char ", &d);

    //printf("Please confirm the selection:\n"
    //        "device number: %d \n"
    //        "channel number: %d \n"
    //        "PWM value: %d \n ",
    //        deviceNum, channelNum, pwmVal);
}

int char_getUserInput(const char* msg, char* dataAddr){
    //if ( (*dataAddr)=='Y') printf ("You entered Y\n");
    //else if ( (*dataAddr)=='n') printf ("You entered n\n");
    //else printf("Not Y or n.\n");
    printf("%s", msg);
    scanf(" %c", dataAddr);
    while( ((*dataAddr)!='Y') &&  ((*dataAddr)!='n')){
        printf("Please re-enter [Y/n]: ");
        scanf(" %c", dataAddr);
    }
    return 0;
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


