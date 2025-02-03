#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  // N�cessaire pour inet_pton()

#pragma comment(lib, "ws2_32.lib")

class Client
{
private:
	SOCKET udpSocket;
	sockaddr_in serverAddr{};
public:
	Client();
	~Client();
	int Connect();
	void Send();
	void Update();
	int Disconnect();
};

