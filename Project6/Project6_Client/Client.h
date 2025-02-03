#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  // N�cessaire pour inet_pton()

#pragma comment(lib, "ws2_32.lib")

class Client
{
private:
	SOCKET udpSocket;
public:
	Client();
	~Client();
	int Connect();
	int Disconnect();
};

