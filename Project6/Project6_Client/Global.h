#pragma once
#include "SFML/Graphics.hpp"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define FIELD_SIZE { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 20 }
#define BUTTON_SIZE { WINDOW_WIDTH / 8, WINDOW_HEIGHT / 10 }

#define BUTTON_HOVER_COLOR { 255, 255, 120 }

#define PLAYER_1_UP sf::Keyboard::Z
#define PLAYER_1_DOWN sf::Keyboard::S
#define PLAYER_2_UP sf::Keyboard::Up
#define PLAYER_2_DOWN sf::Keyboard::Down

#define RACKET_VELOCITY 10.f
#define BALL_DEFAULT_SPEED 2.f
#define BALL_SPEED_UP 1.f
