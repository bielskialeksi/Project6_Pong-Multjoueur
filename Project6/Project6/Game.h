#pragma once
#include "Entity.h"
class Game
{
public:
	Game();
	void Loop();
	void MoveBall();											
	void MoveRacket();
	void CheckWin();

private:
	
	Vector2f m_score;							//x = score player 1 / y = score player 2
	Entity* ball;								//ball principal
	Entity* racket_1;							//raquette player 1
	Entity* racket_2;							//raquette player 2

	float m_BallVelocityX = 2.0f;				//vitesse de la balle en x
	float m_BallyVelocityY = 2.0f;				//vitesse de la balle en y


};

