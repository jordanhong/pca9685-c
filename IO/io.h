int getUserInput(   const char* msg, int* dataAddr, int lowBound, int upBound);
/* @brief   Asks for user input and stores the data at an address provided 
 *          If user input is out of bound, then repeats asking until within boundaries.
 *
 * @param   msg: Character array for the asking message.
 * @param   dataAddr: Data address to store an integer result.
 * @param   lowBound: lower bound of the data.
 * @param   upBound: Upper bound of the data.
 * @return  0 for success
 */

int char_getUserInput(const char* msg, char* dataAddr);
/* @brief   Asks for user input (char). If not Y or n, then reasks input.
 *
 * @param   msg: Character array for the asking message.
 * @param   dataAddr: Data address to store an integer result.
 * @return  0 for success and -1 for error.
 */

int check_fileExists(const char * filename);
/* @brief Checks if the file exists in the directory.
 *
 * @param   filename: filehandle.
 * @return  1 for fileExists and 0 otherwise. 
 */

int writeCommand_2file(const char* path, uint8_t* report, int report_len );
/*  @brief  Writes a command into a text file. Receives the command in the form of an I2c package (report).
 *
 *  @param  path: Path of the text file to write the command into.
 *  @param  report: I2C package, contains [(uint8_t) [commandMode, len_low_byte, len_high_byte, slave address, data]
 *  @param  report_len: length of report array.
 *  @return 0 for success and -1 for failure.
 */

int readFile_to_array( const char* path, uint8_t* result, int len);
/*  @brief  Reads address data from a text file. The file should contain byte-size data in hexadecimal format, e.g. 4a cooresponds to 0x4a.
 *          This is used for reading an address table from a text file.
 *  @param  path: file for reading in data.
 *  @param  result: array that stores the addresses in the textfile.
 *  @return 0 for success and -1 for failure.
 */

int print_array(uint8_t* array, int len);
/*  @brief  Prints items from array.
 *
 *  @param  array: source array.
 *  @param  len:length of source array to print from.
 *  @return 0 for success and -1 for failure
 */




