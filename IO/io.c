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
    if (msg==NULL) return -1;
    if (dataAddr==NULL) return -1;

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
    if (report==NULL) return -1;
    if (report_len<5) return -1;  
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

int readFile_to_array( const char* path, uint8_t* result, int len){
    FILE    *file;


    // Assumes result to be malloc'd
    if (result==NULL) return -1;
    file = fopen(path, "r");
 
    // Read in file line by line
    // Assume hexadecimal context
    // Stores into array
    unsigned int num;
    for (int i=0; i<len; i++){
        fscanf(file, "%x", &num);
        // printf("Read in: %x\n", num);
        
        // Assign to array element
        result[i] = (uint8_t) num;
    }

    fclose(file);
    return 0;

}

int print_array(uint8_t* array, int len){
    if (array==NULL) return -1;
    int i = 0;

    for (i=0;i<len;i++){
        unsigned int num;
        num = (unsigned int) array[i];
        printf("This is the %d element in array: %x.\n", i,num); 
    }
    return 0;
}
