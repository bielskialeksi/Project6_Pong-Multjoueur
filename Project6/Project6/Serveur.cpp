#include "Serveur.h"




Serveur::Serveur()
{
}

Serveur::~Serveur()
{
	if (listenerThread.joinable()) {
		listenerThread.join();  // Attend que le thread se termine proprement
	}
	StopSending();
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
	
	char host[NI_MAXHOST];
	gethostname(host, NI_MAXHOST);  // Récupère le nom de l'hôte

	addrinfo hints{}, * res;
	hints.ai_family = AF_INET;  // On veut une adresse IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(host, nullptr, &hints, &res) == 0) {
		for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
			sockaddr_in* ipv4 = (sockaddr_in*)p->ai_addr;
			char ipStr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &ipv4->sin_addr, ipStr, INET_ADDRSTRLEN);
			std::cout << "Serveur en écoute sur l'adresse locale : " << ipStr << std::endl;
		}
		freeaddrinfo(res);
	}
	else {
		std::cerr << "Erreur : Impossible de récupérer l'adresse locale\n";
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
	if (!listening) {
		listening = true;
		if (listenerThread.joinable()) {
			listenerThread.join();  // On termine l'ancien thread avant d'en créer un nouveau
		}
		listenerThread = std::thread(&Serveur::ListenAndRead, this);
	}

	StartSending();
	return 0;
}

/// <summary>
/// Delete a client in the list of connexion 
/// </summary>
/// <param name="client"></param>
void Serveur::RemoveClientFromList(sockaddr_in client, int indexLobby)
{
	if (compare_addresses(client, ListLobbyTwoPlayers[indexLobby].player1)) {
		AdvDisconect(ListLobbyTwoPlayers[indexLobby].player2);
		delete(ListLobbyTwoPlayers[indexLobby].game);
		ListLobbyTwoPlayers.erase(ListLobbyTwoPlayers.begin() + indexLobby);
	}
	if (compare_addresses(client, ListLobbyTwoPlayers[indexLobby].player2)) {
		AdvDisconect(ListLobbyTwoPlayers[indexLobby].player2);
		delete(ListLobbyTwoPlayers[indexLobby].game);
		ListLobbyTwoPlayers.erase(ListLobbyTwoPlayers.begin() + indexLobby);
	}
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
	if (ListLobbyTwoPlayers.empty()) newLobby.index = 0;
	else newLobby.index = (int)ListLobbyTwoPlayers.size();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(10000, 99999);
	newLobby.code = std::to_string(distrib(gen));
	ListLobbyTwoPlayers.push_back(newLobby);




	rapidjson::Document newDoc;
	newDoc.SetObject(); // Réinitialiser l'objet
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	newDoc.AddMember("Code", rapidjson::Value(newLobby.code.c_str(), allocator), allocator); // Ajouter le membre
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);
	std::string jsonString = buffer.GetString();


	std::cout << jsonString << std::endl;


	sendto(udpSocket, jsonString.c_str(), (int)jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
	std::cout << "Open host\n" << "code:" << newLobby.code << std::endl;

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
	if (!doc.IsObject() || !doc.HasMember("Join") || !doc["Join"].IsObject()) {
		std::cerr << "Erreur JSON : Join est invalide\n";
		return;
	}

	const rapidjson::Value& Join = doc["Join"];  // Récupérer l'objet "join"

	// Afficher le contenu de "join" pour vérifier qu'il est bien ce que tu attends


	// Vérifier si 'code' est un membre de l'objet 'join' et si c'est bien une chaîne de caractères
	if (!Join.HasMember("code") || !Join["code"].IsString()) {
		std::cerr << "Erreur : 'code' est manquant ou n'est pas une chaîne de caractères !" << std::endl;
		return;
	}
	if (!Join.HasMember("name") || !Join["name"].IsString()) {
		std::cerr << "Erreur : 'code' est manquant ou n'est pas une chaîne de caractères !" << std::endl;
		return;
	}

	std::string lobbyCode = Join["code"].GetString();  // Récupérer le code du lobby
	std::string namePlayer = Join["name"].GetString();  // Récupérer le name


	// Recherche du lobby correspondant au code
	bool foundLobby = false;  // Flag pour savoir si un lobby a été trouvé


	for (LobbyTwoPlayers& lobby : ListLobbyTwoPlayers) {
		if (lobby.code == lobbyCode) {
			foundLobby = true;
			if (isNullSockaddr(lobby.player2)) {
				// Si un joueur rejoint, remplir les informations et envoyer une réponse
				lobby.player2 = newclient;
				lobby.Player2Name = namePlayer;
				lobby.game = new Game();
				newDoc.AddMember("HasJoin", lobby.index, allocator);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				newDoc.Accept(writer);
				std::string jsonString = buffer.GetString();
				sendto(udpSocket, jsonString.c_str(), (int)jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
				sendto(udpSocket, jsonString.c_str(), (int)jsonString.size(), 0, (sockaddr*)&lobby.player1, sizeof(lobby.player1));
				AddList(baseclientadr);
				lobby.ready = true;
				std::cout << "join\n";
				return;
			}
			else {
				// Si la salle est pleine
				newDoc.AddMember("Full", 1, allocator);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				newDoc.Accept(writer);
				std::string jsonString = buffer.GetString();
				sendto(udpSocket, jsonString.c_str(), (int)jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
				return;
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
		sendto(udpSocket, jsonString.c_str(), (int)jsonString.size(), 0, (sockaddr*)&newclient, sizeof(newclient));
	}
}




/// <summary>
/// Send a message to other client
/// </summary>
/// <param name="clientadr"></param>
/// <param name="message"></param>
void Serveur::Send()
{
	for (LobbyTwoPlayers& lobby : ListLobbyTwoPlayers) {
		if (lobby.ready) {
			lobby.game->Loop();
			CreateJson(&lobby);
			std::lock_guard<std::mutex> lock(mtx_newJson);
			int clientAddrSize = sizeof(lobby.player1);
			sendto(udpSocket, newJson.c_str(), (int)strlen(newJson.c_str()), 0, (sockaddr*)&lobby.player1, clientAddrSize);
			clientAddrSize = sizeof(lobby.player2);
			sendto(udpSocket, newJson.c_str(), (int)strlen(newJson.c_str()), 0, (sockaddr*)&lobby.player2, clientAddrSize);
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
	return (addr1.sin_family == addr2.sin_family &&
		addr1.sin_port == addr2.sin_port &&
		addr1.sin_addr.s_addr == addr2.sin_addr.s_addr);
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

void Serveur::AdvDisconect(sockaddr_in player)
{
	rapidjson::Document newDoc;
	newDoc.SetObject(); // Initialisation nécessaire
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	newDoc.AddMember("AdvDisconect", 1, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);
	std::string jsonString = buffer.GetString();
	sendto(udpSocket, jsonString.c_str(), (int)jsonString.size(), 0, (sockaddr*)&player, sizeof(player));

}

void Serveur::PlayerMove(sockaddr_in client)
{
	if (doc.Parse(readJson.c_str()).HasParseError()) {
		std::cerr << "Erreur de parsing JSON ! Erreur à l'index : " << doc.GetErrorOffset() << std::endl;
		//std::cerr << "Erreur de type : " << rapidjson::GetParseError_En(doc.GetParseError()) << std::endl;
		return;  // Quitte la fonction en cas d'erreur de parsing
	}
	if (!doc.HasMember("Lobby") || !doc["Lobby"].IsInt()) {
		std::cerr << "Erreur : 'code' est manquant ou n'est pas une chaîne de caractères !" << std::endl;
		return;
	}
	int lobby = doc["Lobby"].GetInt();

	if (ListLobbyTwoPlayers[lobby].ready) {

		if (!doc.HasMember("Move") || !doc["Move"].IsBool()) {
			std::cerr << "Erreur : 'name' est manquant ou n'est pas une chaîne de caractères !" << std::endl;
			return;
		}

		if (compare_addresses(client,ListLobbyTwoPlayers[lobby].player1)) {
			ListLobbyTwoPlayers[lobby].game->MovePlayer(0, doc["Move"].GetBool());
		}
		else if (compare_addresses(client, ListLobbyTwoPlayers[lobby].player2)) {
			ListLobbyTwoPlayers[lobby].game->MovePlayer(1, doc["Move"].GetBool());
		}
		else {
			std::cout << "Try to move in the wrong Lobby\n";
		}
	}


}

/// <summary>
/// Modify json with the information of the Ball
/// </summary>
/// <param name="posBallx"></param>
/// <param name="PosBally"></param>
/// <param name="DirBallx"></param>
/// <param name="DirBally"></param>
void Serveur::CreateJson(LobbyTwoPlayers* lobby)
{
	if (!lobby->game) {
		std::cerr << "Erreur : lobby->game est NULL\n";
		return;
	}
	rapidjson::Document newDoc;
	newDoc.SetObject(); // Initialisation nécessaire
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();

	//Ball
	newDoc.AddMember("Lobby", lobby->index, allocator);
	rapidjson::Value Ball(rapidjson::kObjectType);
	Ball.AddMember("Posx", lobby->game->ball.x, allocator);
	Ball.AddMember("Posy", lobby->game->ball.y, allocator);
	Ball.AddMember("Scalex", lobby->game->ballScale.x, allocator);
	Ball.AddMember("Scaley", lobby->game->ballScale.y, allocator);
	newDoc.AddMember("Ball", Ball, allocator);

	//Player 1
	rapidjson::Value Player1(rapidjson::kObjectType);
	Player1.AddMember("Posx", lobby->game->racket_1.x, allocator);
	Player1.AddMember("Posy", lobby->game->racket_1.y, allocator);
	Player1.AddMember("Scalex", lobby->game->scaleRacket.x, allocator);
	Player1.AddMember("Scaley", lobby->game->scaleRacket.y, allocator);
	newDoc.AddMember("Player1", Player1, allocator);

	//Player 2
	rapidjson::Value Player2(rapidjson::kObjectType);
	Player2.AddMember("Posx", lobby->game->racket_2.x, allocator);
	Player2.AddMember("Posy", lobby->game->racket_2.y, allocator);
	Player2.AddMember("Scalex", lobby->game->scaleRacket.x, allocator);
	Player2.AddMember("Scaley", lobby->game->scaleRacket.y, allocator);
	newDoc.AddMember("Player2", Player2, allocator);


	//Score
	rapidjson::Value Score(rapidjson::kObjectType);
	Score.AddMember("Score1", lobby->game->m_score.x, allocator);
	Score.AddMember("Score2", lobby->game->m_score.y, allocator);
	newDoc.AddMember("Score", Score, allocator);


	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);

	std::string jsonString = buffer.GetString();

	newJson = jsonString;

}

/// <summary>
/// Read Json 
/// </summary>
int Serveur::ListenAndRead()
{
	char buffer[2048];

	int clientAddrSize = sizeof(baseclientadr);
	int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&baseclientadr, &clientAddrSize);
	if (bytesReceived > 0) {
		if (bytesReceived >= sizeof(buffer)) {
			std::cerr << "Erreur : Message trop long\n";
			return 1;
		}
		buffer[bytesReceived] = '\0';
		readJson = buffer;
		std::cout << "Message reçu : " << buffer << "\n";
		//ReadJson();
		doc.Parse(readJson.c_str());
		if (doc.HasParseError()) {
			std::cerr << "Erreur de parsing JSON !" << std::endl;
			listening = false;
			return 1;
		}
		if (!doc.IsObject()) {
			std::cerr << "Erreur : JSON invalide ou incorrect. Contenu: " << readJson << std::endl;
			return 1;
		}

		if (doc.HasMember("host")) {
			std::string code = CreateLobby(baseclientadr, doc["name"].GetString());
			AddList(baseclientadr);
			listening = false;
			return 0;
		}
		else if (doc.HasMember("Join") && doc["Join"].IsObject()) {
			JoinLobby(baseclientadr);
			listening = false;
			return 0;
		}
		else if (doc.HasMember("Disconnect")) {
			std::cout << "kill client...\n";
			RemoveClientFromList(baseclientadr, doc["IndexGame"].GetInt());
			listening = false;
			return 1;
		}
		else if (doc.HasMember("Lobby")) {
			PlayerMove(baseclientadr);
			listening = false;
			return 0;
		}
		// Répondre au client
		std::cout << "Message recu !\n";
	}
	listening = false;
	return 0;

}



void Serveur::StartSending() {
	if (running.load()) return; // Vérifie si le thread est déjà en cours

	running = true;
	senderThread = std::thread([this]() {
		while (running.load()) {
			auto start = std::chrono::high_resolution_clock::now();  // Temps de début

			Send();

			// Calculer le temps écoulé et ajuster le délai
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - start;
			std::this_thread::sleep_for(std::chrono::milliseconds(16) - elapsed);
		}
		});
}

// Arrête le thread proprement
void Serveur::StopSending() {
	running = false;
	if (senderThread.joinable()) {
		senderThread.join();
	}
}