#include "server.h"

bool server_running = true;

WSADATA wsa_data;

SOCKET server_socket;

struct sockaddr_in server;


int num_clients = 0;

pthread_t threads[MAX_CLIENTS];

ThreadData thread_data[MAX_CLIENTS];


int init_wsa()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
		return 1;
	}

	return 0;
}

int init_server()
{

	// Create Socket

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		printf("Could not create socket. Error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Configure server addr struc

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// Bind to socket

	if (bind(server_socket, (struct sockaddr * ) & server,  sizeof(server)) == SOCKET_ERROR)
	{
		printf("Failed to bind to socket. Errror: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	return 0;
}

int start_server()
{
	if (init_wsa() == 1)
	{
		return 1;
	}

	if (init_server() == 1)
	{
		return 1;
	}

	// listen for incomming connections
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Failed to listen. Errror: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	printf("[SERVER] Server is listening on port %d\n", PORT);

	while (server_running)
	{
		SOCKET client_socket;
		struct sockaddr_in client;
		int client_size = sizeof(client);
		client_socket = accept(server_socket, (struct sockaddr * ) &client, &client_size);
		if (client_socket == INVALID_SOCKET)
		{
			printf("Accept failed. Error: %d\n", WSAGetLastError());
			continue;
		}
		printf("[SERVER] New connection established with client\n");

		get_client_ip_address(&client_socket, &client, &client_size);

		create_thread(&client_socket);

	}

	for (int i = 0; i < num_clients; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void * handle_client(void * arg)
{
	ThreadData * data = (ThreadData * ) arg;


	char buffer[BUFFER_SIZE];
	int bytes_read;

	bool client_running = true;

	while (client_running)
	{
		memset(buffer, 0, BUFFER_SIZE);
		bytes_read = recv(*data->client_socket, buffer, BUFFER_SIZE, 0);

		if (bytes_read <= 0) {
			printf("[WARNING] Client disconnected disgracefully\n");
			break;
		}

		printf("[CLIENT] %s\n", buffer);
	}
	
	return NULL;
}


void create_thread(SOCKET * client_socket)
{
	thread_data[num_clients].thread_id = num_clients;
	thread_data[num_clients].client_socket = client_socket;

	int rc = pthread_create(&threads[num_clients], NULL, handle_client, &thread_data[num_clients]);

	if (rc) {
		printf("Error creating thread\n");
		return; 
	}

	num_clients ++;
}


void get_client_ip_address(SOCKET * client_socket, struct sockaddr_in * client, int * client_size)
{
	if (getpeername(*client_socket, (struct sockaddr * ) client, client_size) == SOCKET_ERROR)
	{
		printf("getpeername failed: %d\n", WSAGetLastError());
		return;
	}

	char ip_address [INET_ADDRSTRLEN];

	// Convert the IP address to a string
	if (inet_ntop(AF_INET, &client->sin_addr, ip_address, INET_ADDRSTRLEN) == NULL)
	{
		printf("inet_ntop failed\n");
		return;
	}
	printf("Client IP address: %s\n", ip_address);

}