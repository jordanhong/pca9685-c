#include <stdio.h>
#include "io.h"
int getUserInput(const char* msg, int* dataAddr, int lowBound, int upBound){

    printf("%s", msg);
    printf("The value should range between %d and %d: ", lowBound, upBound);
    scanf("%d", dataAddr);
    // Error check input
    while ( ((*dataAddr)<lowBound) || ((*dataAddr)>upBound) ) {
        printf("Entered value not in range [%d, %d], please re-enter: ", lowBound, upBound);
        scanf("%d", dataAddr);
    }

    return 0;
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
int check_fileExists(const char * filename){
   /*
     * Check if a file exist using fopen() function
     * return 1 if the file exist otherwise return 0
     */

    /* try to open file to read */
    FILE *file;
    file = fopen(filename, "r");
    if (file){
        fclose(file);
        return 1;
    }
    return 0;
}

int writeCommand_2file(const char* path, uint8_t* report, int report_len ){
    /* report is the I2C package: (uint8_t) [commandMode, len_low_byte, len_high_byte, slave address, data]
     */
    FILE* fp = NULL;
    int fileExists = -1;
   

    /* If file doesn't exist, create one with headers */
    fileExists = check_fileExists(path);
    if (!fileExists){
       fp = fopen(path, "w");
       fprintf (fp, "Proxy for I2C Write\n");
       fprintf (fp, "Driver handle/ len / SLAVE addr / REG addr/ REG data \n");
       fclose(fp);
    }

    /* open the file for appending*/
    fp = fopen (path, "a");
    fprintf (fp, "0x%x / 0x%x / 0x%x / 0x%x / 0x%x\n", report[0], report[1], report [2], report[3], report[4] );
  
    /* close the file*/  
    fclose (fp);
    return 0;

}
