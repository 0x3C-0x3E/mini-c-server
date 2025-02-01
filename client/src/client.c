#include "client.h"

bool client_running;

WSADATA wsa_data;

SOCKET client_socket;

struct sockaddr_in server;

const char * SERVER_IP;

int PORT;

int init_wsa()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
		return 1;
	}

	return 0;
}


int init_client()
{
	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET)
	{
		printf("Could not create socket. Error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	// convert server ip addr from string to binary form
	if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr.s_addr) <= 0)
	{
		perror("Invalid server IP address");
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Failed to connect. Error: %d\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	printf("[INFO] Connected to server %s:%d\n", SERVER_IP, PORT);

	return 0;
}

int start_client(const char * server_ip, int port)
{
	SERVER_IP = server_ip;
	PORT = port;

	if (init_wsa() == 1)
	{
		return 1;
	}

	if (init_client() == 1)
	{
		return 1;
	}

	while (true)
	{
		char message[BUFFER_SIZE];
		scanf(" %[^\n]", message);
		send(client_socket, message, strlen(message), 0);
	}

	closesocket(client_socket);
	WSACleanup();

	return 0;

}