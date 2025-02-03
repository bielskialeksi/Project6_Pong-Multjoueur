#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class Serveur
{
private:
	SOCKET udpSocket;
	sockaddr_in clientAddr;
	sockaddr_in serverAddr{};
public:
	Serveur();
	~Serveur();
	int Begin();
	int Update();
	int Stop();
};

