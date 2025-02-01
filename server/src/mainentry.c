#include <stdio.h>
#include "server.h"


int main(int argc, char argv[])
{
	printf("Hello, World!\n");
	if (start_server() == 1)
	{
		return 1;
	}
	return 0;
}