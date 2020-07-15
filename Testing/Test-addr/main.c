#include <stdio.h>
int printArray(int* array, int len);
int setArray(int* array, int len, int val);


int main(void){
    int a[10];
    int i;
    printf("the size of an integer in my version of C is %d\n", sizeof(int));
    printf("a is %d\n",a);
   for (i=0;i<10;i=i+1){
      printf(" the ADDRESS OF a[%d] is %d\n",i, &(a[i]));
      a[i] = 0;
   }

    
    int j = 3;
    int* address = a;

    printf("a is %d\n",a);
    printf("adding %d\n", j*sizeof(int));
    address = a + j;
    printf( "The address of index j=%d is %d\n", j, address);
    // *address= 5;

    printf("Element at index %d is %d.\n", j, a[j]);
}

int printArray(int* array, int len){
    if (array==NULL) return -1;
    for (int i=0;i<len; i++){
        printf("Array element index %d has value %d\n", i, array[i]);
    }
    return 0;
}
int setArray(int* array, int len, int val){
    if (array==NULL) return -1;
    for (int i=0;i<len;i++){
        array[i] = val;
    }
    return 0;
}

