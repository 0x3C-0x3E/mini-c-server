#include <stdio.h>
#include "server.h"


int main(int argc, char argv[])
{
	if (argc != 3)
	{
		printf("Not enough args!\n Usage: ./client ipaddr port")
		return;
	}

	const char * server_ip = argv[1];
	int port = atio(argv[2]);

	if (start_client(server_ip, port) == 1)
	{
		return 1;
	}
	return 0;
}