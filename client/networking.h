
#pragma once
#define CONNECTION_FAILURE -1

typedef struct {
	struct sockaddr_in* SocketAddr;
	int TcpSocket;

	enum {
		CONNECTION_STATE_CONNECTED,
		CONNECTION_STATE_DISCONNECTED
	} ConnectionState;
} TcpClient;

TcpClient* InitTcpClient(char* ip, int port);
int ConnectToServer(TcpClient* client);
int SendString(TcpClient* client, char* str);
char* ReceiveData(TcpClient* client, size_t bufferlen);
void InitializeNetworking();
void UninitializeNetworking();




