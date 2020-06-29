// Top level functions (calculating PWM counter)
int calcPWM(int PWM, int delay, int width, int* onCount, int*OffCount); 
int convPWM(int onCount, int offCount, uint8_t LED_ON_H, uint8_t LED_ON_L, uint8_t LED_OFF_H, uint8_t LED_OFF_L);

// PCA9685 Driver

// Device Address
/*Get device num (1-7), convert to address, and inform the I2C bus
 *--> probably doesn't go here
 */

// Low level: writing to registers
int writePWM(uint8_t* 4_ledAddr, uint8_t* 4_ledData);
int writeReg(uint8_t* redAddr, uint8_t* regData);



// Auxilary arithmetic functions
int roundNo(float num);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


