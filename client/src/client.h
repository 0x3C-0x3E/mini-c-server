#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <pthread.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024


int init_wsa();

int init_client();

int start_client(const char * server_ip, int port);

void * revc_thread(void * arg);

void * send_thread(void * arg);

extern bool client_running;

extern WSADATA wsa_data;

extern SOCKET client_socket;

extern struct sockaddr_in server;

extern const char * SERVER_IP;

extern int PORT;