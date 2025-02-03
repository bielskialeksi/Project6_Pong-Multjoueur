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

    int clientAddrSize = sizeof(clientAddr);
    int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Message reçu : " << buffer << "\n";

        // Si le client envoie "exit", on arrête le serveur
        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Fermeture du serveur UDP...\n";
            return 1;
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
