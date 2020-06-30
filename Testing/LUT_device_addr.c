#include <stdio.h>
#include <stdint.h>

uint8_t PCA_getDeviceAddr(int deviceNum);

int main(void){
    int device = 2;
    uint8_t address;
    address = PCA_getDeviceAddr(device);
    printf("%d\n", device);
    printf("%x\n",address);
}

uint8_t PCA_getDeviceAddr(int deviceNum){
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

    uint8_t addTable [7] ={
        0x40, 0x41, 0x42, 0x44, 0x48, 0x50, 0x60
    };

    if ( (deviceNum<lowBound) || (deviceNum>upBound) ){
        return 0x00;
    }
    else{
        return addTable[deviceNum-1];
    }
}

