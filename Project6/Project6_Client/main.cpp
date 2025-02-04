#include <SFML/Graphics.hpp>

#include "Client.h"
int main() {
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML depuis dossier projet !");
    Client client;
    client.Connect();


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                client.Disconnect();
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                client.Send();
            }

        }
        client.Update(0,0);
        window.clear(sf::Color::Red);
        window.display();
    }
    client.~Client();
    return 0;
}