#pragma once
#include <string>
#include "Vector2f.h"
class Game
{
public:
	Game();
	void Loop();
	void MovePlayer(int playerNum,bool MoveUp );
	Vector2f m_score;							//x = score player 1 / y = score player 2
	Vector2f ball;								//ball principal
	Vector2f ballScale;								//ball principal
	Vector2f racket_1;							//raquette player 1
	Vector2f racket_2;							//raquette player 2
	Vector2f scaleRacket;							//raquette player 1
private:
	void MoveBall();											
	void CheckWin();
	
	Vector2f resetBall;

	float m_BallVelocityX = 2.0f;				//vitesse de la balle en x
	float m_BallyVelocityY = 2.0f;				//vitesse de la balle en y


};

