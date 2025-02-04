#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#include <vector>
#include <random>
#include <string>

struct LobbyTwoPlayers {
	sockaddr_in player1;
	sockaddr_in player2;
	std::string code;
};
struct LobbyFourPlayers {
	sockaddr_in player1;
	sockaddr_in player2;
	sockaddr_in player3;
	sockaddr_in player4;
	std::string code;
};

class Serveur
{
private:
	sockaddr_in serverAddr{};
	SOCKET udpSocket;
	sockaddr_in baseclientadr;
	std::vector<sockaddr_in> clientAddr;
	std::vector<LobbyTwoPlayers> ListLobbyTwoPlayers;
	std::vector<LobbyFourPlayers> ListLobbyFourPlayers;

	void CreateJson(int posBallx, int PosBally , int DirBallx , int DirBally);
	void ReadJson();
public:
	Serveur();
	~Serveur();
	int Begin();
	int Update();
	int Stop();
	void AddList(sockaddr_in newclient);

	const char* CreateLobby(sockaddr_in newclient);
	void JoinLobby(sockaddr_in newclient, const char* message);

	void Send(sockaddr_in client , const char* message );
	bool compare_addresses(const sockaddr_in& addr1, const sockaddr_in& addr2);
	bool isNullSockaddr(const sockaddr_in& addr);
};

