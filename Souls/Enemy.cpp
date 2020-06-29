#include "Enemy.h"

Enemy::Enemy(int x, int y, Animator a, Stage stage) : Actor(x, y, a, stage) {
	xaxis = 1;
	SetCollisionRect(x, y, 16, 32);
	type = "Enemy";
	player_x = 0;
	player_y = 0;
	alertness = 150;
	rush_dist = 70;
	approach_dist = 40;
	melee_dist = 30;
	approach_speed = .5;
	rush_speed = 1.3;
	lurch_speed = 1;
	gravity = .20;

	timer_inc = 1;
	attack_timer = 0;
	attack_timer_max = 75;
	stun_timer = 0;
	stun_timer_max = 50;
	stun_timer_max_hit_shield = 25;

	damage = 2;

	hp_max = 5;
	hp = hp_max;


	STATE = idle;
};

void Enemy::Update(float time) {
	center_x = x + wid / 2;
	center_y = y + hig / 2;
	dist_to_player = sqrtf(pow((center_x - player_x), 2) + pow((center_y - player_y), 2));

	switch (alert) {
	case 0:
		STATE = idle;
		if (dist_to_player < alertness) {
			alert = 1;
			xaxis = Functions::sgn(player_x - x);
			facing = xaxis;
			STATE = draw;
		}
		break;

	case 1:
		// DEATH
		if (STATE == dead) {
			break;
		}

		if (hp <= 0 && STATE != dead) {
			hspd = 0;
			STATE = die;
			if (!animator.GetIsPlaying()) {
				animator.SetIsPlaying();
				STATE = dead;
			}
		}

		// SHEALTH SWORD
		if (STATE == shealth) {

		}

		// DRAWING SWORD
		if (STATE == draw) {
			if (!animator.GetIsPlaying()) {
				animator.SetIsPlaying();
				STATE = idle;
			}
		}

		// HIT
		if (STATE == hit) {
			hspd = 0;
			if (stun_timer > stun_timer_max*.75) {
				hspd = hit_dir * -1 * lurch_speed*time;
			}

			stun_timer -= timer_inc * time;
			if (stun_timer < 0) {
				STATE = idle;
			}
		}

		// STUNNED
		if (STATE == stunned) {
			hspd = hit_dir * -1 * lurch_speed * time;
			stun_timer -= timer_inc * time;
			if (stun_timer < 0) {
				STATE = idle;
			}
		}

		// ATTACKING
		if (STATE == attack) {
			int x_offset = 0;
			if (facing < 0) {
				x_offset = 6;
			}

			hspd = 0;

			if (currentFrame > 1 && currentFrame < 2) {
				hspd = facing * lurch_speed * time;
			}
			else if (currentFrame > 2 && currentFrame < 3) {
				attack_collision_box.left = x + ((22 + x_offset) * facing);
				attack_collision_box.top = y + 1;
				attack_collision_box.width = 21;
				attack_collision_box.height = 27;
			}

			if (!animator.GetIsPlaying()) {
				animator.SetIsPlaying();
				STATE = idle;
				attack_timer = attack_timer_max;
			}
		}

		// NORMAL
		if (STATE == idle || STATE == rush || STATE == approach) {
			xaxis = Functions::sgn(player_x - x);
			facing = xaxis;
			hspd = 0;

			if (dist_to_player < melee_dist) {
				attack_timer -= timer_inc * time;
				if (attack_timer <= 0) {
					STATE = attack;
				}
			}
			else {
				if (STATE == rush) {
					hspd = rush_speed * time * xaxis;

					if (dist_to_player < approach_dist) {
						STATE = approach;
					}
				}
				else if (STATE == approach) {
					hspd = approach_speed * time * xaxis;

					if (dist_to_player > rush_dist) {
						STATE = rush;
					}
				}
				else {
					STATE = rush;
				}
			}
		}
		break;
	}

	vspd += gravity * time;
	if (vspd > 2) {
		vspd = 2;
	}

	x += hspd;
	ReactiveCollision(1);

	y += vspd;
	ReactiveCollision(0);

	Animate();
};

void Enemy::Animate() {
	switch (STATE) {
	case idle:
		if (alert) {
			animator.set("Alert");
		}
		else {
			animator.set("Idle");
		}
		break;
	case draw:
		animator.set("DrawS");
		break;
	case shealth:
		animator.set("SheathS");
		break;
	case attack:
		animator.set("Chop");
		break;
	case stunned:
		animator.set("Stunned");
		break;
	case approach:
		animator.set("WalkC");
		break;
	case rush:
		animator.set("RunC");
		break;
	case hit:
		animator.set("Hit");
		break;
	case die:
		animator.set("Die");
		break;
	case dead:
		animator.set("Dead");
		break;
	}
}

void Enemy::Draw(sf::RenderWindow &window, float time) {
	if (false) {
		sf::RectangleShape rect;
		if (STATE == hit) {
			rect.setFillColor(sf::Color(255, 0, 255, 255));
		}
		else {
			rect.setFillColor(sf::Color::Red);
		}
		rect.setSize(sf::Vector2f(wid, hig));
		rect.setPosition(x, y);
		window.draw(rect);

		if (STATE == attack) {
			if (currentFrame > 2 && currentFrame < 3) {
				rect.setFillColor(sf::Color(255, 0, 0, 150));
				rect.setSize(sf::Vector2f(attack_collision_box.width, attack_collision_box.height));
				rect.setPosition(attack_collision_box.left, attack_collision_box.top);
				window.draw(rect);
			}
		}
	}

	if (facing < 0) {
		animator.flip();
	}

	currentFrame = animator.draw(window, time, x + wid / 2, y);
};