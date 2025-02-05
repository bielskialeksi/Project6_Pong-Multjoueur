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
		readJson = buffer;
		std::cout << "Message reçu : " << buffer << "\n";
		//ReadJson();
		doc.Parse(readJson.c_str());
		if (doc.HasParseError()) {
			std::cerr << "Erreur de parsing JSON !" << std::endl;
			return 1;
		}

		if (doc.HasMember("host") ) {
			std::string code = CreateLobby(baseclientadr , doc["name"].GetString());
			sendto(udpSocket, code.c_str(), strlen(code.c_str()), 0, (sockaddr*)&clientAddr, clientAddrSize);
			std::cout << "Open host\n" << "code:" << code<<std::endl;
			return 0;
		}
		else if (doc.HasMember("join") && doc["join"].IsObject()) {
			std::cout << "join\n";
			JoinLobby(baseclientadr);
			return 0;
		}
		else if (doc.HasMember("Disconnect")) {
			std::cout << "kill client...\n";
			RemoveClientFromList(baseclientadr);
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
std::string Serveur::CreateLobby(sockaddr_in newclient, std::string name)
{
	LobbyTwoPlayers newLobby;
	memset(&newLobby.player1, 0, sizeof(sockaddr_in));
	memset(&newLobby.player2, 0, sizeof(sockaddr_in));
	newLobby.player1 = newclient;
	newLobby.Player1Name = name;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(10000, 99999);
	newLobby.code = std::to_string(distrib(gen));
	ListLobbyTwoPlayers.push_back(newLobby);
	return newLobby.code;
}

/// <summary>
/// Make a Player join a lobby 
/// </summary>
/// <param name="newclient"></param>
/// <param name="message"></param>
void Serveur::JoinLobby(sockaddr_in newclient)
{
	// Créer le document pour la réponse
	rapidjson::Document newDoc;
	newDoc.SetObject(); // Assurer que c'est un objet au départ
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();

	// Assure-toi que doc est bien parsé avant d'accéder à ses membres
	if (doc.Parse(readJson.c_str()).HasParseError()) {
		std::cerr << "Erreur de parsing JSON ! Erreur à l'index : " << doc.GetErrorOffset() << std::endl;
		//std::cerr << "Erreur de type : " << rapidjson::GetParseError_En(doc.GetParseError()) << std::endl;
		return;  // Quitte la fonction en cas d'erreur de parsing
	}

	// Vérification si le document 'join' existe et est bien un objet
	if (!doc.HasMember("join") || !doc["join"].IsObject()) {
		std::cerr << "Erreur : 'join' est manquant ou n'est pas un objet !" << std::endl;
		return;
	}

	const rapidjson::Value& Code = doc["join"];  // Récupérer l'objet "join"

	// Afficher le contenu de "join" pour vérifier qu'il est bien ce que tu attends


	// Vérifier si 'code' est un membre de l'objet 'join' et si c'est bien une chaîne de caractères
	if (!Code.HasMember("code") || !Code["code"].IsString()) {
		std::cerr << "Erreur : 'code' est manquant ou n'est pas une chaîne de caractères !" << std::endl;
		return;
	}

	std::string lobbyCode = Code["code"].GetString();  // Récupérer le code du lobby

	// Recherche du lobby correspondant au code
	bool foundLobby = false;  // Flag pour savoir si un lobby a été trouvé


	for (LobbyTwoPlayers& lobby : ListLobbyTwoPlayers) {
		if (lobby.code == lobbyCode) {
			foundLobby = true;
			if (isNullSockaddr(lobby.player2)) {
				// Si un joueur rejoint, remplir les informations et envoyer une réponse
				lobby.player2 = newclient;
				newDoc.AddMember("HasJoin", 0, allocator);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				newDoc.Accept(writer);
				std::string jsonString = buffer.GetString();
				sendto(udpSocket, jsonString.c_str(), jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
				break;
			}
			else {
				// Si la salle est pleine
				newDoc.AddMember("Full", 1, allocator);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				newDoc.Accept(writer);
				std::string jsonString = buffer.GetString();
				sendto(udpSocket, jsonString.c_str(), jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
				break;
			}
		}
	}

	// Si aucun lobby n'a été trouvé, on ajoute "NotFound"
	if (!foundLobby) {
		// Assurer que newDoc est réinitialisé avant de l'utiliser
		std::cout << "NotFound\n";
		newDoc.SetObject(); // Réinitialiser l'objet
		newDoc.AddMember("NotFound", 1, allocator); // Ajouter le membre
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		newDoc.Accept(writer);
		std::string jsonString = buffer.GetString();
		sendto(udpSocket, jsonString.c_str(), jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
	}
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
	rapidjson::Document newDoc;
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	rapidjson::Value Ball(rapidjson::kObjectType);
	Ball.AddMember("Posx", ballPosx, allocator);
	Ball.AddMember("Posy", ballPosx, allocator);
	Ball.AddMember("Dirx", Dirx, allocator);
	Ball.AddMember("Diry", Diry, allocator);
	newDoc.AddMember("Ball", Ball, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);

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
