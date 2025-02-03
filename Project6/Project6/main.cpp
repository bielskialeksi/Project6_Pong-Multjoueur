#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de cr�ation du socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
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

    char buffer[1024];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    while (true) {
        int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Message re�u : " << buffer << "\n";

            // Si le client envoie "exit", on arr�te le serveur
            if (strcmp(buffer, "exit") == 0) {
                std::cout << "Fermeture du serveur UDP...\n";
                break;
            }

            // R�pondre au client
            const char* response = "Message re�u !";
            sendto(udpSocket, response, strlen(response), 0, (sockaddr*)&clientAddr, clientAddrSize);
        }
    }

    closesocket(udpSocket);
    WSACleanup();
    std::cout << "Serveur arr�t� proprement.\n";
    return 0;
}
