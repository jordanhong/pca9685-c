#include <stdio.h>
#include <stdint.h>
int check_fileExists(const char * filename);
int write2file(const char* path, const char * mode, int* Arr, int ArrLen);
int writeCommand_2file(const char* path, uint8_t* report, int report_len );

int main (){
    const char* path = "proxy_MCP.txt";
    const char* mode = "w";
    uint8_t report[5] = {0xF5, 0x04, 29, 0x01, 0x00};
    writeCommand_2file(path, report, 5);
    return 0;
}
int check_fileExists(const char * filename){
   /*
     * Check if a file exist using fopen() function
     * return 1 if the file exist otherwise return 0
     */

    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r")){
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
       fprintf (fp, "Command Mode / len LOW BYTE / len HIGH BYTE / SLAVE addr / DATA \n");
       fclose(fp);
    }

    /* open the file for appending*/
    fp = fopen (path, "a");
    fprintf (fp, "0x%x / 0x%x / 0x%x / 0x%x / 0x%x\n", report[0], report[1], report [2], report[3], report[4] );
  
    /* close the file*/  
    fclose (fp);
    return 0;

}


int write2file(const char* path, const char * mode, int* Arr, int ArrLen){
    FILE * fp;
    int i;
 
    /* open the file for writing*/
    fp = fopen (path, mode);
  
    /* write 10 lines of text into the file stream*/
    for(i = 0; i < ArrLen;i++){
        fprintf (fp, "This is line %d\n", Arr[i]);
    }
  
    /* close the file*/  
    fclose (fp);
    return 0;
}
