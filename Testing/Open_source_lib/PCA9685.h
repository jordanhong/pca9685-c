#include <stdint.h>
#include "libmcp2221.h"
typedef struct PCA PCA;


struct PCA{
    int deviceNum;
    uint8_t address;
    int pwmVal[16];
    int onCount[16];
    int offCount[16];
    int current_channelNum; //hold current channel of attention
    int current_pwmVal;        // hold pwm value for current channel
    mcp2221_t** mcp_dev;
};

typedef enum{
    SINGLE=0,
    BATCH=1,
} writePWM_state;

#define LED_0_start 6
#define LED_multiplier 4


int PCA_configDevice(PCA* pointerPCA, mcp2221_t** mcp_dev);
int PCA_regDeviceAddr(PCA* pointerPCA);
int PCA_regI2cDriver(PCA* pointerPCA, mcp2221_t** mcp_dev);
int PCA_init(PCA* pointerPCA);
int PCA_reset(PCA* pointerPCA);
int PCA_writePWM(PCA* pointerPCA, writePWM_state mode);
int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int PWM);
int PCA_writeReg(PCA* pointerPCA, uint8_t regAddr, uint8_t regData);
int PCA_calcPWM(PCA* pointerPCA, int channelNum, int pwmVal);
int calcPWM( int PWM, int delay, int width, int* onCount, int* offCount);
int getChannelReg(int chNum, uint8_t* regList);
int roundNo(float num);
