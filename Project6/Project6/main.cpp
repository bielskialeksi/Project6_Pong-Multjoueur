#include "Serveur.h"
#include <chrono>
#include "Global.h"

int main() {

	Serveur serveur;
	serveur.Begin();

	const std::chrono::milliseconds frameDuration(1000 / SERVER_FRAMERATE);
	auto lastTime = std::chrono::high_resolution_clock::now();

	while (true)
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);
		if (deltaTime >= frameDuration)
		{
			serveur.Update();
			lastTime = now;
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
	serveur.Stop();
}
