#include "Game.h"
#include "Global.h"
#include "Vector2f.h"

Game::Game()
{
	m_BallVelocityX = BALL_DEFAULT_SPEED;
	m_BallyVelocityY = BALL_DEFAULT_SPEED;

	resetBall = Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	ball = resetBall;
	ballScale = Vector2f(50, 50);

	racket_1 = Vector2f(50, 200);
	racket_2 = Vector2f(WINDOW_WIDTH - 80, 200);

	scaleRacket = Vector2f(30, 100);
}

void Game::Loop()               //Boucle de jeu
{
	MoveBall();
}

void Game::MovePlayer(int playerNum, bool MoveUp)
{
	switch (playerNum)
	{
	case 0:
		if (MoveUp && racket_1.y > 0) {
			racket_1.y = racket_1.y - RACKET_VELOCITY;
		}
		else if (racket_1.y < WINDOW_HEIGHT) {
			racket_1.y = racket_1.y - RACKET_VELOCITY;
		}
		break;
	case 1:
		if (MoveUp && racket_2.y > 0) {
			racket_2.y = racket_2.y - RACKET_VELOCITY;
		}
		else if (racket_2.y < WINDOW_HEIGHT) {
			racket_2.y = racket_2.y - RACKET_VELOCITY;
		}
		break;

	default:
		break;
	}
}

void Game::MoveBall()           //deplace la balle en x & y
{
	Vector2f pos = ball;
	Vector2f scale = ballScale;

	if (m_BallVelocityX > 0) {
		if (ball.x > racket_2.x - scaleRacket.x) {
			if (ball.y< racket_2.y + (scaleRacket.y / 2) && ball.y > racket_2.y - (scaleRacket.y / 2)) {
				m_BallVelocityX += BALL_SPEED_UP;
				m_BallyVelocityY += BALL_SPEED_UP / 2;
				m_BallVelocityX *= -1;
			}
		}
	}
	else {
		if (ball.x < racket_1.x + scaleRacket.x) {
			if (ball.y< racket_1.y + (scaleRacket.y / 2) && ball.y > racket_1.y - (scaleRacket.y / 2)) {
				m_BallVelocityX -= BALL_SPEED_UP;
				m_BallyVelocityY -= BALL_SPEED_UP / 2;
				m_BallVelocityX *= -1;
			}
		}
	}
	if (pos.y + m_BallyVelocityY < 0 || pos.y + m_BallyVelocityY > WINDOW_HEIGHT - scale.y)
		m_BallyVelocityY *= -1;                                                                  //Change la direction de la balle en y

	ball = ball + Vector2f(m_BallVelocityX, m_BallyVelocityY);
	CheckWin();
}


void Game::CheckWin()                                                                   //Teste si la balle va sortir des écran latérale. Si positif : +1 au score du gagnant & reset la pos et vitesse de la balle
{
	Vector2f pos = ball;
	Vector2f scale = ballScale;

	//ecran droit = victoire player 1
	if (pos.x + m_BallVelocityX > WINDOW_WIDTH - scale.x)
	{
		m_score.x += 1;
		ball = resetBall;
		m_BallVelocityX = BALL_DEFAULT_SPEED;
		m_BallyVelocityY = BALL_DEFAULT_SPEED;
		m_BallVelocityX *= -1;
	}

	//ecran gauche = victoire player 2
	else if (pos.x + m_BallVelocityX < 0)
	{
		m_score.y += 1;
		ball = resetBall;
		m_BallVelocityX = BALL_DEFAULT_SPEED;
		m_BallyVelocityY = BALL_DEFAULT_SPEED;
		m_BallVelocityX *= -1;
	}
}
