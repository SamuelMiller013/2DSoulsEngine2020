#pragma once
#include "AnimationEngine.h"
#include "Level.h"
#include "stdafx.h"
#include "Functions.h";

class Actor {
public:
	enum A_TYPES { None, Player, Enemy };
	std::string type = "None";
	float x, y,wid, hig,  xaxis, yaxis, hspd, vspd, currentFrame, center_x, center_y, hit_dir;
	int facing;
	bool invulnerable;
	std::vector<Object> obj;
	Animator animator;
	sf::Texture tex;

	Actor(int px, int py, Animator a, Stage s);

	virtual void Update(float time) = 0;
	virtual void Draw(sf::RenderWindow &window, float time) = 0;

	sf::FloatRect getCollisionRect();
	
	void SetCollisionRect(int px, int py, int pwid, int phig);

	void ReactiveCollision(int num);
};
