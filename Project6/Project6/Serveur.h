#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#include <vector>
#include <random>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>



struct LobbyTwoPlayers {
	int index;
	sockaddr_in player1;
	std::string Player1Name;

	sockaddr_in player2;
	std::string Player2Name;

	std::string code;
};
struct LobbyFourPlayers {
	int index;
	sockaddr_in player1;
	std::string Player1Name;

	sockaddr_in player2;
	std::string Player2Name;

	sockaddr_in player3;
	std::string Player3Name;

	sockaddr_in player4;
	std::string Player4Name;

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

	std::string readJson;
	std::string newJson;

	rapidjson::Document doc;


	int ballPosx, ballPosy, Dirx, Diry;

	void CreateJson();
	void ReadJson();

public:
	Serveur();
	~Serveur();
	int Begin();
	int Update();
	void RemoveClientFromList(sockaddr_in client);
	int Stop();
	void AddList(sockaddr_in newclient);

	std::string CreateLobby(sockaddr_in newclient, std::string name);
	void JoinLobby(sockaddr_in newclient);

	void Send(sockaddr_in client , std::string message );
	bool compare_addresses(const sockaddr_in& addr1, const sockaddr_in& addr2);
	bool isNullSockaddr(const sockaddr_in& addr);
};

