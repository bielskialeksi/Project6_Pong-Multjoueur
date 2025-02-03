#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  // Nécessaire pour inet_pton()
#include <thread>

#pragma comment(lib, "ws2_32.lib")

class Client
{
private:
	SOCKET udpSocket;
	sockaddr_in serverAddr{};
	void Listen();
public:
	Client();
	~Client();
	int Connect();
	void Send();
	void Update();
	int Disconnect();
};

