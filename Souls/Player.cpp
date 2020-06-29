#include "Player.h"

Player::Player(int x, int y, Animator a, Stage stage) : Actor(x, y, a, stage) {
	SetCollisionRect(x, y, 16, 31);
	type = "Player";

	// STATE
	STATE = idle;
	facing = 1;

	// MOVEMENT
	move_speed = 1;
	run_speed = 2;
	dash_speed = 3;
	max_dash_speed = 3;
	lurch_speed = 1;
	jump_speed = 5;
	gravity = .8;

	// COMBAT/STATS
	attack_stage = 1;
	damage = 3;
	hp_max = 10;
	hp = hp_max;
	stm_max = 10;
	stm = stm_max;
	take_damage = 0;

	// TIMERS
	timer_inc = 1;
	stun_timer = 0;
	stun_timer_max = 50;
	stun_timer_max_shield = 10;
	stm_recovery_timer = 0;
	stm_recovery_timer_max = 100;
};

float highest_run, lowest_run;
float highest_walk, lowest_walk;

void Player::Update(float time) {
	// MANAGE STATES
	xaxis = right - left;
	yaxis = down - up;
	shield = shield_key;

	if (stm < 0) { stm = 0; }
	if (hp < 0) { hp = 0; }

	// DEATH
	if (STATE == dead) {

	}

	if (hp <= 0 && STATE != dead) {
		hspd = 0;
		invulnerable = 1;
		STATE = die;
		if (!animator.GetIsPlaying()) {
			animator.SetIsPlaying();
			STATE = dead;
		}
	}

	// DASHING
	if (STATE == dashing) {
		invulnerable = 1;
		hspd = Functions::ApproachValue(hspd, 0, .06*time);
		//printf("%f\n",hspd);
		if (hspd == 0) {
			invulnerable = 0;
			STATE = idle;
		}
	}

	// ATTACKING
	if (STATE == attacking) {
		int x_offset = 0;
		if (facing < 0) {
			x_offset = 6;
		}

		hspd = 0;

		switch (attack_stage) {
		case 1:
			if (currentFrame > 1 && currentFrame < 2) {
				hspd = facing * lurch_speed * time;
			}
			else if (currentFrame > 2 && currentFrame < 3) {
				attack_collision_box_1.left = x + ((22 + x_offset) * facing);
				attack_collision_box_1.top = y + 1;
				attack_collision_box_1.width = 21;
				attack_collision_box_1.height = 27;
			}
			break;
		case 2:
			if (currentFrame > 1 && currentFrame < 2) {
				hspd = facing * lurch_speed * time;
			}
			else if (currentFrame > 2 && currentFrame < 3) {
				attack_collision_box_2.left = x - ((21 + x_offset));
				attack_collision_box_2.top = y + 5;
				attack_collision_box_2.width = 63;
				attack_collision_box_2.height = 9;
			}
			break;
		}

		if (attack && currentFrame > 4 && !attack_again) {
			attack_again = true;
		}

		if (!animator.GetIsPlaying()) {
			animator.SetIsPlaying();
			if (attack_again && stm > 0) {
				attack_stage++;
				if (attack_stage > 2) {
					attack_stage = 1;
				}
				attack_again = false;
				stm -= 2;
				stm_recovery_timer = stm_recovery_timer_max;
			}
			else {
				STATE = idle;
				attack_stage = 1;
			}
		}
	}

	// HIT
	if (STATE == hit) {
		invulnerable = 1;
		hspd = 0;
		if (take_damage) {
			if (stun_timer > stun_timer_max*.75) {
				hspd = hit_dir * -1 * lurch_speed*time;
			}
		}
		else {
			hspd = hit_dir * -1 * lurch_speed*time;
		}

		stun_timer -= timer_inc * time;
		if (stun_timer < 0) {
			STATE = idle;
			invulnerable = 0;
			take_damage = 0;
			hit_dir = 0;
		}
	}

	// NORMAL
	if (STATE == idle
		|| STATE == walking
		|| STATE == running
		) {
		// STAMINA RECOVERY
		if (stm < stm_max && !shield_key && STATE != running) {
			if (stm_recovery_timer > 0) {
				stm_recovery_timer -= time * timer_inc;
			}
			else {
				stm += time * .1;
				if (stm >= stm_max) {
					stm = stm_max;
				}
			}
		}

		// MOVEMENT
		hspd = 0;
		STATE = idle;

		if (xaxis != 0) {
			hspd = move_speed * time * xaxis;
			STATE = walking;
			if (!shield) {
				facing = Functions::sgn(xaxis);
				if (run && stm > 0) {
					hspd = run_speed * time * xaxis;
					STATE = running;
					stm -= time * .05;
					stm_recovery_timer = stm_recovery_timer_max;
				}
			}
		}

		if (yaxis != 0) {
			vspd = move_speed * time * yaxis;
		}

		vspd += gravity * time;

		// DASH
		if (have_dashed && !dash) {
			have_dashed = 0;
		}

		if (dash && !have_dashed) {
			STATE = dashing;
			hspd = Functions::Clamp(dash_speed * time * facing, -max_dash_speed, max_dash_speed);

			have_dashed = 1;
			stm -= 4;
			stm_recovery_timer = stm_recovery_timer_max;
			printf("Dashed with time = %f, while facing = %i, for hspd = %f\n", time, facing, hspd);
		}

		// ATTACK
		if (attack && stm > 0) {
			STATE = attacking;
			stm -= 2;
			stm_recovery_timer = stm_recovery_timer_max;
		}
	}

	// APPLY MOVEMENT AND COLLISIONS
	x += hspd;
	ReactiveCollision(1);

	y += vspd;
	ReactiveCollision(0);

	// SET ANIMATION
	Animate();
};

void Player::Animate() {
	switch (STATE) {
	case idle:
		if (shield) {
			animator.set("IdleShield");
		}
		else {
			animator.set("IdleC");
		}
		break;
	case walking:
		if (shield) {
			if (facing == xaxis) {
				animator.set("WalkFShield");
			}
			else {
				animator.set("WalkRShield");
			}
		}
		else {
			animator.set("WalkC");
		}
		//printf("walking hspd = %f\n", hspd);
		break;
	case running:
		animator.set("RunC");
		//printf("running hspd = %f\n", hspd);
		break;
	case dashing:
		animator.set("Dash");
		break;
	case hit:
		animator.set("Hit");
		break;
	case attacking:
		switch (attack_stage) {
		case 1: animator.set("Chop"); break;
		case 2: animator.set("Slash"); break;
		}
		break;
	case die:
		animator.set("Die");
		break;
	case dead:
		animator.set("Dead");
		break;
	}
}

void Player::Draw(sf::RenderWindow &window, float time) {
	// Collision Box Visualizaions
	if (false)
	{
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Blue);
		rect.setSize(sf::Vector2f(wid, hig));
		rect.setPosition(x, y);
		window.draw(rect);

		if (STATE == attacking) {
			if (currentFrame > 2 && currentFrame < 3) {
				rect.setFillColor(sf::Color(255, 0, 0, 150));

				switch (attack_stage) {
				case 1:
					rect.setSize(sf::Vector2f(attack_collision_box_1.width, attack_collision_box_1.height));
					rect.setPosition(attack_collision_box_1.left, attack_collision_box_1.top);
					break;
				case 2:
					rect.setSize(sf::Vector2f(attack_collision_box_2.width, attack_collision_box_2.height));
					rect.setPosition(attack_collision_box_2.left, attack_collision_box_2.top);
					break;
				}
				window.draw(rect);
			}
		}
	}

	if (facing < 0) {
		animator.flip();
	}

	currentFrame = animator.draw(window, time, x + wid / 2, y);
};