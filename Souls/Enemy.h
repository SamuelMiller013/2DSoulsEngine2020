#pragma once
#include "Actor.h"

class Enemy : public Actor {
public:
	float approach_speed, rush_speed, lurch_speed, gravity; 

	float timer_inc, attack_timer, attack_timer_max, stun_timer, stun_timer_max, stun_timer_max_hit_shield;

	float damage, hp_max, hp;

	bool alert;
	float player_x, player_y, alertness, rush_dist, approach_dist, melee_dist, dist_to_player;
	enum { idle, draw, shealth, approach, rush, attack, stunned, hit, die, dead } STATE;
	sf::FloatRect attack_collision_box;

	Enemy(int x, int y, Animator a, Stage stage);

	void Update(float time);

	void Animate();

	void Draw(sf::RenderWindow &window, float time);
};