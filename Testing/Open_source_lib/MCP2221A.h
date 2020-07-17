#include "libmcp2221.h"
typedef struct PCA PCA;

int MCP_init(mcp2221_t** myDev);
int MCP_i2cWrite(PCA* pointerPCA, uint8_t regAddr, uint8_t regData);
int MCP_i2cWrite_proxy(mcp2221_t* device, uint8_t address, void* data, int numByte, mcp2221_i2crw_t type);
