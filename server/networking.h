
#pragma once

typedef struct {
	struct sockaddr_int* SocketAddr;
	int TcpSocket;
} TcpServer;

typedef struct {
	struct sockaddr_in* SocketAddr;
	int TcpSocket;
} TcpClient;

TcpServer* InitTcpServer(char* ip, int port, int max_clients);
void ConnectionListener(TcpServer* server);
TcpClient* AcceptConnection(TcpServer* server);
int SendData(TcpClient* client, char* str);
char* ReceiveData(TcpClient* client, size_t bufferlen);
void InitializeNetworking();
void UninitializeNetworking();

