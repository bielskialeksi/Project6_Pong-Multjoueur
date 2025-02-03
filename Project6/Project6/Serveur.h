#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#include <vector>
class Serveur
{
private:
	sockaddr_in serverAddr{};
	SOCKET udpSocket;
	sockaddr_in baseclientadr;
	std::vector<sockaddr_in> clientAddr;
public:
	Serveur();
	~Serveur();
	int Begin();
	int Update();
	int Stop();
	void AddList(sockaddr_in newclient);

	void Send(sockaddr_in client , const char* message );
	bool compare_addresses(const sockaddr_in& addr1, const sockaddr_in& addr2);
};

