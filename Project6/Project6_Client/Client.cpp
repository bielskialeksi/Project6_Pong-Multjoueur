﻿#include "Client.h"

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
		std::cout << " Message : " << buffer << std::endl;
		jsonToRead = buffer;
	}
	listening = false;
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

	//const char* message = "Hello, serveur UDP!";
	//sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

	std::cout << "Message envoyé au serveur UDP.\n";


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
	std::cout << "JSON envoyé : " << buffer.GetString() << std::endl;

}




/// <summary>
/// Listen the serveur and create next package and send to serveur;
/// </summary>
void Client::Update(int posPadx, int posPady)
{
	if (!listening) {
		listening = true;
		if (listenerThread.joinable()) {
			listenerThread.join();  // On termine l'ancien thread avant d'en créer un nouveau
		}
		listenerThread = std::thread(&Client::Listen, this);
	}
	if (jsonToRead != "") {
		ReadJson();
		jsonToRead.clear();
	}
	/*CreateJson(posPadx, posPady);*/

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
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	sendto(udpSocket, buffer.GetString(), (int)strlen(buffer.GetString()), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	closesocket(udpSocket);
	WSACleanup();
	return 0;
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
	std::cout << jsonToRead << std::endl;
	doc.Parse(jsonToRead.c_str());
	if (doc.HasParseError()) {
		std::cerr << "Erreur de parsing JSON !" << std::endl;
		return;
	}
	if (conected) {
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
			const rapidjson::Value& Ball = doc["Ball"]; // Récupérer l'objet

			// Vérifier si "Posx" et "Posy" existent
			if (Ball.HasMember("Posx") && Ball["Posx"].IsInt()) {
				std::cout << "Posx : " << Ball["Posx"].GetInt() << std::endl;
			}
			if (Ball.HasMember("Posy") && Ball["Posy"].IsInt()) {
				std::cout << "Posy : " << Ball["Posy"].GetInt() << std::endl;
			}

			// Vérifier si "Dirx" et "Diry" existent
			if (Ball.HasMember("Dirx") && Ball["Dirx"].IsInt()) {
				std::cout << "Dirx : " << Ball["Dirx"].GetInt() << std::endl;
			}
			if (Ball.HasMember("Diry") && Ball["Diry"].IsInt()) {
				std::cout << "Diry : " << Ball["Diry"].GetInt() << std::endl;
			}
		}
	}
	else if (doc.HasMember("NotFound") || doc.HasMember("Full")) {
		/// 
	}
	else if (doc.HasMember("Hasjoin")) {
		conected = true;
	}
	else if (doc.HasMember("Code")) {
		clientCode = doc["Code"].GetString();
		std::cout << "get code\n";
	}
}
