#pragma once
#include "Actor.h"

class Player : public Actor {
public:
	// MOVEMENT
	float move_speed, run_speed, dash_speed, max_dash_speed, lurch_speed, jump_speed, gravity, anim_speed;
	// COMBAT/STATS
	float damage, hp, hp_max, stm, stm_max;
	int attack_stage;
	sf::FloatRect attack_collision_box_1, attack_collision_box_2;
	// TIMERS
	float stun_timer, stun_timer_max, stun_timer_max_shield, timer_inc, stm_recovery_timer, stm_recovery_timer_max;
	// STATES
	bool left, right, up, down, jump, run, attack, dash, have_dashed, attack_again, shield_key, shield, take_damage;
	enum { idle, walking, running, dashing, attacking, hit, die, dead } STATE;

	Player(int x, int y, Animator a, Stage stage);

	// DEBUGGING VARIABLES
	float highest_run, lowest_run;
	float highest_walk, lowest_walk;

	void Update(float time);

	void Animate();

	void Draw(sf::RenderWindow &window, float time);
};