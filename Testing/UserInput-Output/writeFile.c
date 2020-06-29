#include <stdio.h>
int write2file(const char* path, const char * mode, int* Arr, int ArrLen);

int main (){
    const char* path = "dataOut.txt";
    const char* mode = "w";
    
    // Sample data
    int arrLen = 10;
    int arrData[10] = {1, 2, 3, 4, 5, 10, 9, 8, 7, 6};

    // call function to write to data
    write2file(path, mode, arrData, arrLen);
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
