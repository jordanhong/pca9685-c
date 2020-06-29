#include <stdio.h>
#include <stdint.h>

#define LED_0_start 6
#define LED_multiplier 4


int print_uint_array(uint8_t* arr, int size);
int getChannelReg(int chNum, uint8_t* regList);

int main(void){
    int chNum = 0;
    uint8_t chReg[4];

    chNum = 16;
    getChannelReg(chNum, chReg);
    print_uint_array(chReg, 4);

}

int print_uint_array(uint8_t* arr, int size){
    int i= 0;

    for (i=0;i<size;i++){
        printf("%x\n", arr[i]);
    }
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

