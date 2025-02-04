#include "Client.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
/// <summary>
/// Listen ifthe serveur send a message 
/// </summary>
void Client::Listen()
{
	char buffer[1024];
	sockaddr_in senderAddr;
	int senderLen = sizeof(senderAddr);

	int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (sockaddr*)&senderAddr, &senderLen);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		std::cout << " Message : " << buffer << std::endl;
	}

}

Client::Client()
{
}

Client::~Client()
{
}
/// <summary>
/// Mqke the first connexion with the serveur
/// </summary>
/// <returns></returns>
int Client::Connect()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET) {
		std::cerr << "Erreur de création du socket\n";
		return 1;
	}


	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50500);  // ⚠️ On utilise 50500
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Adresse du serveur

	const char* message = "Hello, serveur UDP!";
	sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

	std::cout << "Message envoyé au serveur UDP.\n";


	return 0;

}

/// <summary>
/// Send a package to serveur
/// </summary>
void Client::Send()
{
	std::cout << "Client Send" << std::endl;
	const char* message = "Hey";
	sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

/// <summary>
/// Listen the serveur and create next package and send to serveur;
/// </summary>
void Client::Update()
{
	std::thread listenerThread(&Client::Listen, this);
	listenerThread.detach();
}

/// <summary>
/// Disconnect Client 
/// </summary>
/// <returns></returns>
int Client::Disconnect()
{
	const char* message = "Disconnect";
	sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	closesocket(udpSocket);
	WSACleanup();
	return 0;
}

/// <summary>
/// Create a package 
/// </summary>
/// <param name="posPadx"></param>
/// <param name="PosPady"></param>
void Client::CreateJson(int posPadx, int PosPady)
{
}

/// <summary>
/// Read a package
/// </summary>
void Client::ReadJson()
{
}
