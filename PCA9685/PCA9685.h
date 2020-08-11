#include <stdint.h>
typedef struct PCA PCA;


struct PCA{
    int deviceNum;
    uint8_t address;
    int pwmVal[16];
    int onCount[16];
    int offCount[16];
    void** mcp_dev;
};

typedef enum{
    SINGLE=0,
    BATCH=1,
} writePWM_state;

#define LED_0_start 6
#define LED_multiplier 4
#define PCA_address_table {0x40, 0x41, 0x42, 0x44, 0x48, 0x50, 0x60}
#define TOTAL_PCA_NUM 7

/******************* Master functions (Global perspective)****************************/

int PCA_master_init(int numDevices, PCA** arrayHeader);
/** @brief Master function to initialize structs to resemble PCA9685 drivers.
 *
 *  @param numDevices:  Number of PCA9685 devices to initialize.
 *  @param arrayHeader: Pointer to the start of array. Function will malloc according to numDevices, and initialize each struct PCA element. 
 *  @return 0 for success; -1 for failure (in malloc)
 */

int PCA_master_writePWM(PCA* master, writePWM_state mode);
/** @brief  Asks for user input for device/channel/PWM, 
 *          and writes the PWM to the specified channel.
 *
 *  @param  master: Starting address for the struct PCA array.
 *  @param  mode:   Mode to write PWM:
 *                      1) BATCH: Iterates through all devices in array and all channels in devices. Write PWM according to the pwmVal information. 
 *                      2) SINGLE Writes to specific device/channels without changing existing modes.
 *  @return 0 for success and -1 for failure
 */

int PCA_master_restart(PCA* master, int numDevices);
/** @brief: Restarts all drivers by writing 0 PWM to all channels.
 *          1) Calls reset on individual drivers (sets 0 to all PWM); 
 *          2) Calls PCA_master_writePWM on batch mode
 *
 *  @param  master:         Starting address for the struct PCA array.
 *  @param  numDevices:     Number of PCA9685 devices to restart.
 *  @return 0 for success; -1 for failure (in malloc)
 */

int PCA_master_getState(PCA* master, int numDevices);
/** @brief: Prints out states of all devices/channels in array
 *
 *  @param  master:         Starting address for the struct PCA array.
 *  @param  numDevices:     Number of PCA9685 devices in master.
 *  @return 0 for success; -1 for failure (in malloc)
 */


/******************** PCA Driver functions********************************************/
int PCA_reset(PCA* pointerPCA); 
/** @brief  Resets information stored in a PCA struct.
 *
 *  @param  pointerPCA: pointer to a PCA struct.     
 *  @return 0 for success; -1 for failure (in malloc)
 */

int PCA_getState(PCA* pointerPCA);
/** @brief  Gets state of specific device
 *
 * @param   pointerPCA: pointer to a PCA struct
 * @return  0 for success; -1 for failure
 */

int PCA_setPWM(PCA* pointerPCA, int channel, int pwm); 
/** @brief  Calculates counter values. Returns results by updating the struct entry. 
 *          This function only calculates the values and updates the struct. Does NOT drive the PWM registers!. 
 *  @param  pointerPCA: pointer to a PCA struct
 *  @return  0 for success; -1 for failure
 */


int PCA_writePWM_2channel(PCA* pointerPCA, int channelNum, int pwm);
/*  @brief  Writes PWM to channel registers based on parameter in arguments.
 *
 *  @param  pointerPCA: pointer to a PCA struct
 *  @param  channelNum: channel number (1-16)
 *  @param  pwm:PWM value in %.
 */

int PCA_calcPWM(PCA* pointerPCA, int channelNum, int pwmVal); 
/*  @brief  Caculates counters, called by setPWM
 *
 *  @param  pointerPCA: pointer to a PCA struct
 *  @param  channelNum: channel number (1-16)
 *  @param  pwmVal: PWM value in %.
 */

/********************I2C and Register level Functions****************************************************/

int PCA_regI2cDriver(PCA* pointerPCA, void** mcp_dev);
/*  @brief  Registers the USB/I2c converter driver. Currently not implemented.
 *
 *  TODO: This function is currently not programmed. Check the MCP dll for how ow to initialize this.
 *  @param  pointerPCA: pointer to a PCA struct
 *  @param  mcp_dev: pointer to MCP device
 */

int PCA_writeReg(PCA* pointerPCA, uint8_t regAddr, uint8_t regData);
/*
 *  @brief  Write data to register.
 *
 *  @param  regAddr: 8 bit register address.
 *  @param  regData: 8 bit TX data.
 */

int MCP_i2cWrite_proxy(PCA* pointerPCA, unsigned int numBytes, unsigned char* data);
/*  @brief  Proxy function to carry out I2C write. Writes to a text file with the commands
 *
 *  @param  pointerPCA: pointer to a PCA struct
 *  @param  numBytes: Number of bytes to write. 
 *  @param  data: data packet with {8bit register address, 8bit register data}
 *  
 *  @return 0 for success, -1 for failure
 */

int MCP_i2cWrite(PCA* pointerPCA, unsigned int numBytes, unsigned char* data);
/*  @brief  Normal function to issue I2C write. 
 *
 *  @param  pointerPCA: pointer to a PCA struct
 *  @param  numBytes: Number of bytes to write. 
 *  @param  data: data packet with {8bit register address, 8bit register data}
 *  
 *  @return 0 for success, -1 for failure
 */
  
/********************* Auxiliary functions*********************************************/
uint8_t PCA_getDeviceAddr(int deviceNum);
/* @brief   Get programmable address of a PCA device by the device number.
 *
 * Current implementation stores the programmable device in an array.
 * TODO: Consider putting into a text file and read from there.
 *
 * @param   deviceNum: Device number (1-7).
 * @return  8 bit address of PCA device.
 */

int PCA_getChannelReg(int chNum, uint8_t* regList);
/*  @brief  Compute the register numbers according to the channel number (1-16)
 *          Register numbers are stored in an array of 4. In the following order: LED#_ON_L, LED#_OFF_H, LED#_ON_L, LED#_OFF_H
 *  @param  chNum   Channel number (1-16) on a single PCA device.
 *  @param  regList Register list (4 uint8_t) to store the result.
 */


int calcPWM( int PWM, int delay, int width, int* onCount, int* offCount);
/*  @brief  Calculates the counter values for PWM pulse generation. Stores in the pointer address provided in argument.
 *
 *  @param  PWM: PWM value in %. (1-100)
 *  @param  delay: delay (offset of ON) in percentage of the width (1-100)
 *  @param  width: Width of PWM cycle. 
 *  @param  onCount: pointer to store counter value of ON counter. 
 *  @param  offCount: pointer to store counter value of OFF counter.
 *  @return 0 for success and -1 for error.
 */

int roundNo(float num);
/*  @brief  Round up a float to an int using 0.5 threshold.
 *  @param  num: float number to round
 *  @return integer of rounded number.
 */

int zero(int* array, int size);
/*  @brief  Force all elements to zero in an integer array.
 *  @param  array: Array to zero. 
 *  @param  Size of array.
 *  @return 0 for success and -1 for failure
 */



