int MCP_init(mcp2221_t** myDev){

	puts("Starting!");

	mcp2221_init();
	
	// Get list of MCP2221s
	printf("Looking for devices... ");
	int count = mcp2221_find(MCP2221_DEFAULT_VID, MCP2221_DEFAULT_PID, NULL, NULL, NULL);
	printf("found %d devices\n", count);

	// Open whatever device was found first
	printf("Opening device... ");
	// mcp2221_t* myDev = mcp2221_open(); //here myDev is of type mcp2221_t*
	*myDev = mcp2221_open(); // here myDev is of type mcp2221_t**

	if(!(*myDev))
	{
		mcp2221_exit();
		puts("No MCP2221s found");
		getchar();
		return 0;
	}
	puts("done");

	
}
