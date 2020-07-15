#include <stdio.h>

typedef enum{
    SINGLE=0,
    BATCH=1,
} writePWM_state;


int main(void){
    writePWM_state mode = SINGLE;
    switch(mode){
        case(SINGLE):
            printf("SINGLE mode\n");
            break;
        case(BATCH):
            printf("BATCH mode\n");
            break;
    }
    return 0;
}

