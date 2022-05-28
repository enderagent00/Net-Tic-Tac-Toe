
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "networking.h"
#include "debugging.h"
#pragma comment(lib, "WS2_32")
#define SOCKADDR struct sockaddr_in

/*
	Initialize a Tcp Client on the heap
*/
TcpClient* InitTcpClient(char* ip, int port) {
	SOCKADDR addr;
	int sock = 0;
	int valread;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		PRINTERROR("Failed to initialize socket");
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip, &addr.sin_addr) == SOCKET_ERROR) {
		PRINTERROR("Invalid or unsupported IP address specified");
	}

	TcpClient* client = calloc(1, sizeof(TcpClient));
	client->SocketAddr = &addr;
	client->TcpSocket = sock;
	client->ConnectionState = CONNECTION_STATE_DISCONNECTED;

	return client;
}

/*
	Attempts to connect to the server, returns the result of the connection attempt
*/
int ConnectToServer(TcpClient* client) {
	int res = connect(client->TcpSocket, client->SocketAddr, sizeof(SOCKADDR));

	if (res != CONNECTION_FAILURE) {
		client->ConnectionState = CONNECTION_STATE_CONNECTED;
	}

	return res;
}

/*
	Send data to the server
*/
int SendData(TcpClient* client, char* str) {
	int len = send(client->TcpSocket, str, strlen(str), 0);
	Sleep(500);
	return len;
}

/*
	Receive data from the server
*/
char* ReceiveData(TcpClient* client, size_t bufferlen) {
	char* buffer = calloc(bufferlen + 1, sizeof(char));
	int len = recv(client->TcpSocket, buffer, bufferlen, 0);

	if (len == 0 || len == -1) {
		buffer[bufferlen] = '\0';
		client->ConnectionState = CONNECTION_STATE_DISCONNECTED;
	}
	else {
		buffer[len] = '\0';
	}

	return buffer;
}

/*
	Required by winsock before any networking can happen
*/
void InitializeNetworking() {
	WORD version = MAKEWORD(1, 1);
	WSADATA wsa;
	WSAStartup(version, &wsa);
}

void UninitializeNetworking() {
	WSACleanup();
}





