#include "Serveur.h"

int main() {
	Serveur serveur;
	serveur.Begin();
	while (true) {
		serveur.Update();
	}
	serveur.Stop();
}
