int getUserInput(   const char* msg, int* dataAddr, int lowBound, int upBound);
int check_fileExists(const char * filename);
int writeCommand_2file(const char* path, uint8_t* report, int report_len );
int readFile_to_array( const char* path, uint8_t* result, int len);
int print_array(uint8_t* array, int len);

