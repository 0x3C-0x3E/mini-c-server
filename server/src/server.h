#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>


#include <pthread.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int init_wsa();

int init_server();

int start_server();

void handle_client();

void close_socket(int server);

void cleanup();


void get_client_ip_address(SOCKET * client_socket, struct sockaddr_in * client, int * client_size);


extern WSADATA wsa_data;

extern SOCKET server_socket;

extern struct sockaddr_in server;

const int client_size = sizeof(struct sockaddr_in);

extern bool server_running;