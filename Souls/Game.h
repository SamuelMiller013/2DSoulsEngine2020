#pragma once
#include "Level.h"
#include "Player.h"
#include "Enemy.h"

class Game {
public:
	static void Runner();

private:
	static std::vector<Actor*> ActorTypeInRectangle(sf::FloatRect r, std::string t, std::vector<Actor*> &act);
};