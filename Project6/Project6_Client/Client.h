#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  // Nécessaire pour inet_pton()
#include <thread>

#pragma comment(lib, "ws2_32.lib")


#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class Client
{
private:
	bool conected;

	SOCKET udpSocket;
	sockaddr_in serverAddr{};
	std::string jsonToSend="";
	std::string jsonToRead="";

	std::thread listenerThread;
	bool listening = false;

	rapidjson::Document doc;

	int PosPadAdvx;
	int PosPadAdvy;
	int PosBallx;
	int PosBally;
	int DirBallx;
	int DirBally;

	void Listen();
	void CreateJson(int posPadx, int PosPady);
	void ReadJson();
public:
	Client();
	~Client();
	int Connect();
	void Send();
	
	void Host(std::string name);
	void Join(std::string name,std::string code);

	void Update(int posPadx, int PosPady);
	int Disconnect();

};

