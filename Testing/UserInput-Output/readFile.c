#include <stdio.h>
#include <string.h>
#include <stdint.h>
int printFile(const char* path, int len);

int main(){
    const char* path = "address_assignment_2.txt";
    // printFile(path,7);
    uint8_t array[7];

    readFile_to_array(path, array, 7);
    print_array(array, 7);
    return 0;
}
int printFile(const char* path, int len){
    FILE    *file;
    file = fopen(path, "r");
    
    // Return -1 if file cannot be opened
    if (!file) return -1;   

    // Print file content to screen 
    for (int i=0; i<len; i++){
        unsigned int num;
        fscanf(file, "%x", &num);
        printf("Read in: %x\n", num);
    }

    fclose(file);
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
        printf("Read in: %x\n", num);
        
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
