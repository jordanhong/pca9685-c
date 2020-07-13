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

/******************* Master functions (Global perspective)****************************/
int PCA_master_init(int numDevices, PCA** arrayHeader);                 // called at beginning, return array of (numDevices) PCA structs
int PCA_master_writePWM(PCA* master, writePWM_state mode);        // ask for input and set specific device/channel/PWM
                                                     // Two modes:   BATCH  (writes to all registers according to array)
                                                     //              SINGLE (writes to specific device/channels without changing existing modes)
int PCA_master_restart(PCA* master, int numDevices);                            // 1) call reset on individual drivers; 2) Calls setPWM to 0 on batch mode
int PCA_master_getState(PCA* master, int numDevices);                           // Gets state of device; asks which device

/******************** PCA Driver functions********************************************/
int PCA_reset(PCA* pointerPCA); // resets information stored in the PCA struct
int PCA_getState(PCA* pointerPCA); // Gets state of specific device
int PCA_setPWM(PCA* pointerPCA, int channel, int pwm); // Calculates pwm/counter values and update struct
int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int pwm); // writes PWM to channel registers based on values
int PCA_calcPWM(PCA* pointerPCA, int channelNum, int pwmVal); //caculates counters, called by setPWM; TODO(merge this function elsewhere?)

/********************I2C and Register level Functions****************************************************/

int PCA_regI2cDriver(PCA* pointerPCA, void** mcp_dev);
int PCA_writeReg(PCA* pointerPCA, uint8_t regAddr, uint8_t regData);
int MCP_i2cWrite_proxy(PCA* pointerPCA, uint8_t regAddr,uint8_t regData);

/********************* Auxiliary functions*********************************************/
uint8_t PCA_getDeviceAddr(int deviceNum);
int PCA_getChannelReg(int chNum, uint8_t* regList);
int calcPWM( int PWM, int delay, int width, int* onCount, int* offCount);
int roundNo(float num);
int zero(int* array, int size);


