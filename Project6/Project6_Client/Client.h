#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  // Nécessaire pour inet_pton()
#include <thread>
#include <string>
#pragma comment(lib, "ws2_32.lib")


#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class Client
{
private:
	bool listenServeur = false;
	bool conected;
	int lobby;

	SOCKET udpSocket;
	sockaddr_in serverAddr{};
	std::string jsonToSend="";
	std::string jsonToRead="";

	std::thread listenerThread;
	std::atomic<bool> running{ true };
	bool listening = false;

	std::string clientCode;

	rapidjson::Document doc;
	void Listen();
	void CreateJson(int posPadx, int PosPady);
	void ReadJson();

	void Shutdown();
public:
	int PosPLayer1x;
	int PosPLayer1y;

	int PosPLayer2x;
	int PosPLayer2y;
	
	int PosBallx;
	int PosBally;

	int score1;
	int score2;

	Client();
	~Client();
	int Connect();
	void Send();
	
	void Host(std::string name);
	void Join(std::string name,std::string code);

	void Update();
	int Disconnect();

	void Move(bool UpOrDown);

	bool IsConected() { return conected; };
	std::string GetCodeClient();
};

