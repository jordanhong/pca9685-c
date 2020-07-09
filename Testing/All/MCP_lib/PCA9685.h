#include <stdint.h>
typedef struct PCA PCA;


struct PCA{
    int deviceNum;
    uint8_t address;
    int pwmVal[16];
    int onCount[16];
    int offCount[16];
    int current_channelNum; //hold current channel of attention
    int current_pwmVal;        // hold pwm value for current channel
    void** mcp_dev;
};

typedef enum{
    SINGLE=0,
    BATCH=1,
} writePWM_state;

#define LED_0_start 6
#define LED_multiplier 4


int PCA_regI2cDriver(PCA* pointerPCA, void** mcp_dev);
int PCA_init(PCA** ppPCA);
int PCA_reset(PCA* pointerPCA);
uint8_t PCA_getDeviceAddr(int deviceNum);
int PCA_writePWM(PCA* pointerPCA, writePWM_state mode);
int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int PWM);
int PCA_writeReg(PCA* pointerPCA, uint8_t regAddr, uint8_t regData);
int PCA_calcPWM(PCA* pointerPCA, int channelNum, int pwmVal);
int PCA_getState(PCA*pointerPCA);
int calcPWM( int PWM, int delay, int width, int* onCount, int* offCount);
int getChannelReg(int chNum, uint8_t* regList);
int roundNo(float num);
int MCP_i2cWrite_proxy(PCA* pointerPCA, uint8_t regAddr,uint8_t regData);
