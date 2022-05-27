
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include "networking.h"
#include "debugging.h"
#pragma comment(lib, "WS2_32")
#define SOCKADDR struct sockaddr_in

TcpServer* InitTcpServer(char* ip, int port, int max_clients) {
	SOCKADDR addr;
	int sock = 0;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		PRINTERROR("Failed to initialize socket");
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(sock, &addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		PRINTERROR("Failed to bind socket");
	}

	listen(sock, max_clients);

	TcpServer* server = calloc(1, sizeof(TcpServer));
	server->SocketAddr = &addr;
	server->TcpSocket = sock;

	return server;
}

TcpClient* AcceptConnection(TcpServer* server) {
	SOCKADDR clientaddr;
	int addrlen = sizeof(SOCKADDR);
	int client_socket = accept(server->TcpSocket, &clientaddr, &addrlen);
	
	TcpClient* client = calloc(1, sizeof(TcpClient));
	client->SocketAddr = &clientaddr;
	client->TcpSocket = client_socket;

	return client;
}

int SendString(TcpClient* client, char* str) {
	int len = send(client->TcpSocket, str, strlen(str), 0);
	Sleep(500);
	return len;
}

char* ReceiveData(TcpClient* client, size_t bufferlen) {
	char* buffer = calloc(bufferlen + 1, sizeof(char));
	int len = recv(client->TcpSocket, buffer, bufferlen, 0);

	if (len == 0 || len == -1) {
		buffer[bufferlen] = '\0';
	}
	else {
		buffer[len] = '\0';
	}

	return buffer;
}


void InitializeNetworking() {
	WORD version = MAKEWORD(1, 1);
	WSADATA wsa;
	WSAStartup(version, &wsa);
}

void UninitializeNetworking() {
	WSACleanup();
}


