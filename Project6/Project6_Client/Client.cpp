#include "Client.h"

/// <summary>
/// Listen ifthe serveur send a message 
/// </summary>
void Client::Listen()
{
	char buffer[1024];
	sockaddr_in senderAddr;
	int senderLen = sizeof(senderAddr);
	int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&senderAddr, (socklen_t*)&senderLen);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		//std::cout << " Message : " << buffer << std::endl;
		if (!running) return;
		jsonToRead = buffer;
	}
	listening = false;
}

Client::Client()
{
}

Client::~Client()
{

	Shutdown();
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

	// Lire l'adresse IP depuis le fichier config.txt
	std::string ipAddress = readIPAddressFromFile("config.txt");
	if (ipAddress.empty() || !isValidIPAddress(ipAddress)) {
		std::cerr << "Adresse IP invalide ou absente dans le fichier.\n";
		closesocket(udpSocket);
		WSACleanup();
		return 1;
	}

	// Configurer l'adresse du serveur avec l'IP lue
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50500);  // Port du serveur
	inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);  // Convertir l'adresse IP lue

	//const char* message = "Listen serveur";
	//int result = sendto(udpSocket, message, (int)strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//if (result == SOCKET_ERROR) {
	//	std::cerr << "Erreur d'envoi : " << WSAGetLastError() << "\n";
	//	closesocket(udpSocket);
	//	WSACleanup();
	//	return 1;
	//}
	return 0;
}

/// <summary>
/// Send a package to serveur
/// </summary>
void Client::Send()
{
	//std::cout << "Client Send" << std::endl;
	sendto(udpSocket, jsonToSend.c_str(), (int)strlen(jsonToSend.c_str()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}


/// <summary>
/// Host
/// </summary>
void Client::Host(std::string name)
{
	rapidjson::Document newDoc;
	newDoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	newDoc.AddMember("host", 0, allocator);
	rapidjson::Value Name;
	Name.SetString(name.c_str(), allocator);
	newDoc.AddMember("name", Name, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);

	sendto(udpSocket, buffer.GetString(), (int)strlen(buffer.GetString()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}


/// <summary>
/// Join
/// </summary>
void Client::Join(std::string name,std::string code)
{
	rapidjson::Document newDoc;
	newDoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	rapidjson::Value Join(rapidjson::kObjectType);

	rapidjson::Value codeValue;
	codeValue.SetString(code.c_str(), allocator);
	Join.AddMember("code", codeValue, allocator);

	rapidjson::Value nameValue;
	nameValue.SetString(name.c_str(), allocator);
	Join.AddMember("name", nameValue, allocator);

	newDoc.AddMember("Join", Join, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);

	sendto(udpSocket, buffer.GetString(), (int)strlen(buffer.GetString()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//std::cout << "JSON envoyé : " << buffer.GetString() << std::endl;

}




/// <summary>
/// Listen the serveur and create next package and send to serveur;
/// </summary>
void Client::Update()
{
	if (!listening && running) {  // Vérifie que le thread peut être lancé
		listening = true;
		if (listenerThread.joinable()) {
			listenerThread.join();  // Attend la fin du thread précédent
		}
		listenerThread = std::thread(&Client::Listen, this);
	}

	if (!jsonToRead.empty()) {
		ReadJson();
		jsonToRead.clear();
	}
}

/// <summary>
/// Disconnect Client 
/// </summary>
/// <returns></returns>
int Client::Disconnect()
{
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("Disconnect", 0, allocator);
	doc.AddMember("IndexGame", lobby, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	sendto(udpSocket, buffer.GetString(), (int)strlen(buffer.GetString()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	Shutdown();
	return 0;
}

void Client::Move(bool UpOrDown)
{
	rapidjson::Document newDoc;
	newDoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();
	newDoc.AddMember("Lobby", lobby, allocator);
	newDoc.AddMember("Move", UpOrDown, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);
	sendto(udpSocket, buffer.GetString(), (int)strlen(buffer.GetString()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}


std::string Client::GetCodeClient()
{
	return clientCode;
}

/// <summary>
/// Create a package 
/// </summary>
/// <param name="posPadx"></param>
/// <param name="PosPady"></param>
void Client::CreateJson(int posPadx, int PosPady)
{
	rapidjson::Document newDoc;
	newDoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();

	rapidjson::Value playerAdv(rapidjson::kObjectType);
	playerAdv.AddMember("Posx", posPadx, allocator);
	playerAdv.AddMember("Posy", PosPady, allocator);
	newDoc.AddMember("PlayerAdv", playerAdv, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	newDoc.Accept(writer);

	std::string jsonString = buffer.GetString();

	jsonToSend = jsonString;
}

/// <summary>
/// Read a package
/// </summary>
void Client::ReadJson()
{
	//std::cout << jsonToRead << std::endl;
	doc.SetObject();
	doc.Parse(jsonToRead.c_str());
	if (doc.HasParseError()) {
		std::cerr << "Erreur de parsing JSON !" << std::endl;
		return;
	}
	if (conected) {
		if (doc.HasMember("AdvDisconect")) {
			conected = false;
			return;
		}
		//Read pos Player1
		if (doc.HasMember("Player1") && doc["Player1"].IsObject()) {
			const rapidjson::Value& Racket1 = doc["Player1"]; // Récupérer l'objet
			// Vérifier si "Posx" et "Posy" existent
			if (Racket1.HasMember("Posx") && Racket1["Posx"].IsFloat()) {
				PosPLayer1x = Racket1["Posx"].GetFloat();
			}
			if (Racket1.HasMember("Posy") && Racket1["Posy"].IsFloat()) {
				PosPLayer1y = Racket1["Posy"].GetFloat();;
			}
		}
		//pos Player2
		if (doc.HasMember("Player2") && doc["Player2"].IsObject()) {
			const rapidjson::Value& Racket2 = doc["Player2"]; // Récupérer l'objet

			// Vérifier si "Posx" et "Posy" existent
			if (Racket2.HasMember("Posx") && Racket2["Posx"].IsFloat()) {
				PosPLayer2x = Racket2["Posx"].GetFloat();
			}
			if (Racket2.HasMember("Posy") && Racket2["Posy"].IsFloat()) {
				PosPLayer2y = Racket2["Posy"].GetFloat();
			}
		}
		//Read pos Ball 
		if (doc.HasMember("Ball") && doc["Ball"].IsObject()) {
			const rapidjson::Value& Ball = doc["Ball"]; // Récupérer l'objet

			// Vérifier si "Posx" et "Posy" existent
			if (Ball.HasMember("Posx") && Ball["Posx"].IsFloat()) {
				PosBallx = Ball["Posx"].GetFloat();
			}
			if (Ball.HasMember("Posy") && Ball["Posy"].IsFloat()) {
				PosBally = Ball["Posy"].GetFloat();
			}
		}
		if (doc.HasMember("Score") && doc["Score"].IsObject()) {
			const rapidjson::Value& Score = doc["Score"]; // Récupérer l'objet

			// Vérifier si "Posx" et "Posy" existent
			if (Score.HasMember("Score1") && Score["Score1"].IsFloat()) {
				score1= Score["Score1"].GetFloat();
			}
			if (Score.HasMember("Score2") && Score["Score2"].IsFloat()) {
				score2 = Score["Score2"].GetFloat();
			}
		}
	}
	else if (doc.HasMember("NotFound") || doc.HasMember("Full")) {
		/// 
	}
	else if (doc.HasMember("HasJoin")) {
		conected = true;
	}
	else if (doc.HasMember("Code")) {
		clientCode = doc["Code"].GetString();
		//::cout << "get code\n";
	}
}


void Client::Shutdown()
{
	conected = false;
	running = false;  // Arrête le thread d'écoute proprement

	if (udpSocket != INVALID_SOCKET) {
		closesocket(udpSocket);
		udpSocket = INVALID_SOCKET;
	}


	if (listenerThread.joinable()) {
		try {
			listenerThread.join();  // Attendre la fin du thread
		}
		catch (const std::exception& e) {
			std::cerr << "Erreur lors de la fermeture du thread : " << e.what() << std::endl;
		}
	}


	if (doc.IsObject() && !doc.ObjectEmpty()) {
		doc.RemoveAllMembers(); // Supprime tous les éléments de l'objet JSON
	}

	std::cout << "Client fermé proprement.\n";

	assert(udpSocket == INVALID_SOCKET && "Tentative de fermeture d'un socket déjà fermé !");

}

std::string Client::readIPAddressFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	std::string ip;

	if (file.is_open()) {
		std::getline(file, ip);
		file.close();
	}
	else {
		std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << "\n";
		return "";
	}

	return ip;
}



bool Client::isValidIPAddress(const std::string& ip)
{
	sockaddr_in sa;
	return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
}