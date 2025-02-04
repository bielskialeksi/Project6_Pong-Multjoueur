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
		jsonToRead = buffer;
	}

}

Client::Client()
{
}

Client::~Client()
{
	if (listenerThread.joinable()) {
		listenerThread.join();  // Attend que le thread se termine proprement
	}
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
	//std::cout << "Client Send" << std::endl;
	sendto(udpSocket, jsonToSend.c_str(), strlen(jsonToSend.c_str()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

/// <summary>
/// Listen the serveur and create next package and send to serveur;
/// </summary>
void Client::Update(int posPadx, int posPady)
{
	CreateJson(posPadx, posPady);
	if (!listening) {  // Vérifie si un thread est déjà lancé
		listening = true;
		listenerThread = std::thread(&Client::Listen, this);
	}
	if (jsonToRead != "") {
		ReadJson();
		jsonToRead.clear();
	}
	Send();
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
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value playerAdv(rapidjson::kObjectType);
	playerAdv.AddMember("Posx", posPadx, allocator);
	playerAdv.AddMember("Posy", PosPady, allocator);
	doc.AddMember("PlayerAdv", playerAdv, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string jsonString = buffer.GetString();

	jsonToSend = jsonString;
}

/// <summary>
/// Read a package
/// </summary>
void Client::ReadJson()
{
	rapidjson::Document doc;
	doc.Parse(jsonToRead.c_str());

	if (doc.HasParseError()) {
		std::cerr << "Erreur de parsing JSON !" << std::endl;
		return;
	}

	//Read pos ADV
	if (doc.HasMember("PlayerAdv") && doc["PlayerAdv"].IsObject()) {
		const rapidjson::Value& playerAdv = doc["PlayerAdv"]; // Récupérer l'objet

		// Vérifier si "Posx" et "Posy" existent
		if (playerAdv.HasMember("Posx") && playerAdv["Posx"].IsInt()) {
			std::cout << "Posx : " << playerAdv["Posx"].GetInt() << std::endl;
		}
		if (playerAdv.HasMember("Posy") && playerAdv["Posy"].IsInt()) {
			std::cout << "Posy : " << playerAdv["Posy"].GetInt() << std::endl;
		}
	}

	//Read pos Ball 
	if (doc.HasMember("Ball") && doc["Ball"].IsObject()) {
		const rapidjson::Value& playerAdv = doc["Ball"]; // Récupérer l'objet

		// Vérifier si "Posx" et "Posy" existent
		if (playerAdv.HasMember("Posx") && playerAdv["Posx"].IsInt()) {
			std::cout << "Posx : " << playerAdv["Posx"].GetInt() << std::endl;
		}
		if (playerAdv.HasMember("Posy") && playerAdv["Posy"].IsInt()) {
			std::cout << "Posy : " << playerAdv["Posy"].GetInt() << std::endl;
		}

		// Vérifier si "Dirx" et "Diry" existent
		if (playerAdv.HasMember("Posx") && playerAdv["Posx"].IsInt()) {
			std::cout << "Posx : " << playerAdv["Posx"].GetInt() << std::endl;
		}
		if (playerAdv.HasMember("Posy") && playerAdv["Posy"].IsInt()) {
			std::cout << "Posy : " << playerAdv["Posy"].GetInt() << std::endl;
		}
	}

}
