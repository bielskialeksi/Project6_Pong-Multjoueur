#include "Serveur.h"

Serveur::Serveur()
{
}

Serveur::~Serveur()
{
}

int Serveur::Begin()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET) {
		std::cerr << "Erreur de création du socket\n";
		return 1;
	}


	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50500);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(udpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Erreur de bind\n";
		closesocket(udpSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Serveur UDP en écoute sur le port 50500...\n";
	return 0;
}

int Serveur::Update()
{
	char buffer[1024];

	int clientAddrSize = sizeof(baseclientadr);
	int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&baseclientadr, &clientAddrSize);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		std::cout << "Message reçu : " << buffer << "\n";

		// Si le client envoie "exit", on arrête le serveur
		if (strcmp(buffer, "exit") == 0) {
			std::cout << "Fermeture du serveur UDP...\n";
			return 1;
		}
		else {
			AddList(baseclientadr);
			for (sockaddr_in client : clientAddr) {
				if (compare_addresses(client, baseclientadr)) {
					Send(client, buffer);
				}
			}
		}
		// Répondre au client
		const char* response = "Message recu !";
		sendto(udpSocket, response, strlen(response), 0, (sockaddr*)&clientAddr, clientAddrSize);
	}
	return 0;
}

int Serveur::Stop()
{
	closesocket(udpSocket);
	WSACleanup();
	return 0;
}

void Serveur::AddList(sockaddr_in newclient)
{
	for (const auto& addr : clientAddr) {
		if (compare_addresses(addr, newclient)) {
			std::cout << "Adresse déjà présente." << std::endl;
			return;
		}
	}

	// Si l'adresse n'est pas trouvée, on l'ajoute
	clientAddr.push_back(newclient);
	std::cout << "Adresse ajoutée." << std::endl;
}

void Serveur::Send(sockaddr_in clientadr, const char* message)
{
	std::cout << "Serveur Send" << std::endl;
	for (sockaddr_in client : clientAddr) {
		if (!compare_addresses(client, clientadr)) {
			int clientAddrSize = sizeof(client);
			sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&client, clientAddrSize);
		}
	}
}

bool Serveur::compare_addresses(const sockaddr_in& addr1, const sockaddr_in& addr2) {
	// Comparaison des adresses IP et des ports
	return (addr1.sin_family == addr2.sin_family) &&
		(addr1.sin_port == addr2.sin_port) &&
		(memcmp(&addr1.sin_addr, &addr2.sin_addr, sizeof(addr1.sin_addr)) == 0);
}