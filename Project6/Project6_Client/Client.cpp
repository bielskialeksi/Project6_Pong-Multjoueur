#include "Client.h"


Client::Client()
{
}

Client::~Client()
{
}

int Client::Connect()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET) {
		std::cerr << "Erreur de création du socket\n";
		return 1;
	}

	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50500);  // ⚠️ On utilise 50500
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Adresse du serveur

	const char* message = "Hello, serveur UDP!";
	sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

	std::cout << "Message envoyé au serveur UDP.\n";


	return 0;

}

void Client::Update()
{
}

int Client::Disconnect()
{
	closesocket(udpSocket);
	WSACleanup();
	return 0;
}
