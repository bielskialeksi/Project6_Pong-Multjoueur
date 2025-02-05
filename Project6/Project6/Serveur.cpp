#include "Serveur.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>



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
		std::cerr << "Erreur de création du socket\n";
		WSACleanup();
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
		std::cout << "Message reçu : " << buffer << "\n";

		// Si le client envoie "exit", on arrête le serveur
		ReadJson();

		if (doc.HasParseError()) {
			std::cerr << "Erreur de parsing JSON !" << std::endl;
			return;
		}

		if (doc.HasMember("host") ) {
			std::cout << "Open host\n";
			std::string code = CreateLobby(baseclientadr);
			sendto(udpSocket, code.c_str(), strlen(code.c_str()), 0, (sockaddr*)&clientAddr, clientAddrSize);
			return;
		}
		else if (doc.HasMember("join") && doc["join"].IsObject()) {
			std::cout << "join\n";
			JoinLobby(baseclientadr);
			return;
		}
		else if (doc.HasMember("Disconnect")) {
			std::cout << "kill client...\n";
			RemoveClientFromList(baseclientadr);
			return;
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

/// <summary>
/// Delete a client in the list of connexion 
/// </summary>
/// <param name="client"></param>
void Serveur::RemoveClientFromList(sockaddr_in client)
{
	auto it = std::find_if(clientAddr.begin(), clientAddr.end(),
		[&](const sockaddr_in& addr) { return compare_addresses(addr, client); });
	if (it != clientAddr.end()) {
		clientAddr.erase(it);  // Supprimer le client de la liste
		std::cout << "Client supprimé de la liste.\n";
	}
}

/// <summary>
/// Stop serveur
/// </summary>
/// <returns></returns>
int Serveur::Stop()
{
	if (udpSocket != INVALID_SOCKET) {
		closesocket(udpSocket);
		udpSocket = INVALID_SOCKET;  // Marque le socket comme fermé
	}
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
			std::cout << "Adresse déjà présente." << std::endl;
			return;
		}
	}

	// Si l'adresse n'est pas trouvée, on l'ajoute
	clientAddr.push_back(newclient);
	std::cout << "Adresse ajoutée." << std::endl;
}


/// <summary>
/// Create a Lobby of two players
/// </summary>
/// <param name="newclient"></param>
std::string Serveur::CreateLobby(sockaddr_in newclient)
{
	LobbyTwoPlayers newLobby;
	memset(&newLobby.player1, 0, sizeof(sockaddr_in));
	memset(&newLobby.player2, 0, sizeof(sockaddr_in));
	newLobby.player1 = newclient;
	ListLobbyTwoPlayers.push_back(newLobby);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(10000, 99999);
	newLobby.code = std::to_string(distrib(gen));
	return newLobby.code;
}

/// <summary>
/// Make a Player join a lobby 
/// </summary>
/// <param name="newclient"></param>
/// <param name="message"></param>
void Serveur::JoinLobby(sockaddr_in newclient)
{
	//Base 
	rapidjson::Document newDoc;
	newDoc.Parse(readJson.c_str());
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	if (doc.HasParseError()) {
		std::cerr << "Erreur de parsing JSON !" << std::endl;
		return;
	}

	const rapidjson::Value& Code = doc["join"]; // Récupérer l'objet
	for (LobbyTwoPlayers lobby : ListLobbyTwoPlayers) {
		if (lobby.code == Code["code"].GetString()) {
			if (isNullSockaddr(lobby.player2))
				lobby.player2 = newclient;
			else {
				newDoc.AddMember("Full", 1, allocator);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				newDoc.Accept(writer);

				std::string jsonString = buffer.GetString();
				sendto(udpSocket, jsonString.c_str(), strlen(jsonString.c_str()), 0, (sockaddr*)&newclient, sizeof(newclient));
				break;
			}
		}
	}

	newDoc.AddMember("NotFound", 1, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);

	std::string jsonString = buffer.GetString();
	sendto(udpSocket, jsonString.c_str(), strlen(jsonString.c_str()), 0, (sockaddr*)&newclient, sizeof(newclient));

	// make a system with code 
}



/// <summary>
/// Send a message to other client
/// </summary>
/// <param name="clientadr"></param>
/// <param name="message"></param>
void Serveur::Send(sockaddr_in clientadr, std::string message)
{
	std::cout << "Serveur Send" << std::endl;

	CreateJson();
	for (sockaddr_in client : clientAddr) {
		if (!compare_addresses(client, clientadr)) {
			int clientAddrSize = sizeof(client);
			sendto(udpSocket, newJson.c_str(), strlen(newJson.c_str()), 0, (sockaddr*)&client, clientAddrSize);
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


/// <summary>
/// Verify if a sockaddr_in is non initialize or equal to 0
/// </summary>
/// <param name="addr"></param>
/// <returns></returns>
bool Serveur::isNullSockaddr(const sockaddr_in& addr) {
	return addr.sin_family == 0 &&
		addr.sin_port == 0 &&
		addr.sin_addr.s_addr == 0;
}

/// <summary>
/// Modify json with the information of the Ball
/// </summary>
/// <param name="posBallx"></param>
/// <param name="PosBally"></param>
/// <param name="DirBallx"></param>
/// <param name="DirBally"></param>
void Serveur::CreateJson()
{
	doc.Parse(readJson.c_str());

	if (doc.HasParseError()) {
		std::cerr << "Erreur de parsing JSON !" << std::endl;
		return;
	}
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value Ball(rapidjson::kObjectType);
	Ball.AddMember("Posx", ballPosx, allocator);
	Ball.AddMember("Posy", ballPosx, allocator);
	Ball.AddMember("Dirx", Dirx, allocator);
	Ball.AddMember("Diry", Diry, allocator);
	doc.AddMember("Ball", Ball, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string jsonString = buffer.GetString();

	newJson = jsonString;
}

/// <summary>
/// Read Json 
/// </summary>
void Serveur::ReadJson()
{
	doc.Parse(readJson.c_str());

}
