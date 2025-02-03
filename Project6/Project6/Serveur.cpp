#include "Serveur.h"

Serveur::Serveur()
{
}

Serveur::~Serveur()
{
}
/// <summary>
/// create Serveur
/// </summary>
/// <returns></returns>
int Serveur::Begin()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET) {
		std::cerr << "Erreur de cr�ation du socket\n";
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

	std::cout << "Serveur UDP en �coute sur le port 50500...\n";
	return 0;
}


/// <summary>
/// get the message and send to other client 
/// </summary>
/// <returns></returns>
int Serveur::Update()
{
	char buffer[1024];

	int clientAddrSize = sizeof(baseclientadr);
	int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&baseclientadr, &clientAddrSize);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		std::cout << "Message re�u : " << buffer << "\n";

		// Si le client envoie "exit", on arr�te le serveur
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
		// R�pondre au client
		const char* response = "Message recu !";
		sendto(udpSocket, response, strlen(response), 0, (sockaddr*)&clientAddr, clientAddrSize);
	}
	return 0;
}

/// <summary>
/// Stop serveur
/// </summary>
/// <returns></returns>
int Serveur::Stop()
{
	closesocket(udpSocket);
	WSACleanup();
	return 0;
}

/// <summary>
/// Verify if it's a new client and add it 
/// </summary>
/// <param name="newclient"></param>
void Serveur::AddList(sockaddr_in newclient)
{
	for (const auto& addr : clientAddr) {
		if (compare_addresses(addr, newclient)) {
			std::cout << "Adresse d�j� pr�sente." << std::endl;
			return;
		}
	}

	// Si l'adresse n'est pas trouv�e, on l'ajoute
	clientAddr.push_back(newclient);
	std::cout << "Adresse ajout�e." << std::endl;
}


/// <summary>
/// Create a Lobby of two players
/// </summary>
/// <param name="newclient"></param>
void Serveur::CreateLobby(sockaddr_in newclient)
{
}

/// <summary>
/// Make a Player join a lobby 
/// </summary>
/// <param name="newclient"></param>
/// <param name="message"></param>
void Serveur::JoinLobby(sockaddr_in newclient, const char* message)
{
}



/// <summary>
/// Send a message to other client
/// </summary>
/// <param name="clientadr"></param>
/// <param name="message"></param>
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

/// <summary>
/// Compare the adress of enter two sockaddr_in
/// </summary>
/// <param name="addr1"></param>
/// <param name="addr2"></param>
/// <returns></returns>
bool Serveur::compare_addresses(const sockaddr_in& addr1, const sockaddr_in& addr2) {
	// Comparaison des adresses IP et des ports
	return (addr1.sin_family == addr2.sin_family) &&
		(addr1.sin_port == addr2.sin_port) &&
		(memcmp(&addr1.sin_addr, &addr2.sin_addr, sizeof(addr1.sin_addr)) == 0);
}