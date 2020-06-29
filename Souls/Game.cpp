#include "Game.h"

void Game::Runner() {
	sf::RenderWindow window(sf::VideoMode(768, 768), "Vania");
	sf::View view(sf::FloatRect(0, 0, 256, 256));

	sf::Font font;
	if (font.loadFromFile("fonts/AGoblinAppears-o2aV.ttf")) {
		printf("Successfully fetched font.\n");
	}

	Stage stage;
	stage.buildLevelFromTMX("levels/test_level_2.tmx");

	//ANIMATIONS
	Animator animator;
	sf::Texture tex;
	float anim_speed = .1;

	if (!tex.loadFromFile("images/All_Sprites.png")) {
		std::cout << "Failed loaded sprite texture" << std::endl;
	}

	// Sword Chop
	animator.create("Chop", tex, 0, 399, 50, 31, 6, 15, 0, 0, 0, anim_speed, false);
	// Die
	animator.create("Die", tex, 0, 438, 50, 33, 14, 0, 1, 0, 1, anim_speed, false);
	// Dead
	animator.create("Dead", tex, 650, 438, 50, 33, 1, 0, 1, 0, 1, anim_speed, false);
	// Draw sword
	animator.create("DrawS", tex, 0, 471, 24, 39, 4, 0, 0, 0, 0, anim_speed, false);
	// Hit
	animator.create("Hit", tex, 0, 519, 26, 31, 1, 0, 0, 0, 0, anim_speed, true);
	// Idle Combat
	animator.create("IdleC", tex, 0, 559, 20, 31, 3, 7, 0, 0, 0, anim_speed / 4.5, true);
	// Run Combat
	animator.create("RunC", tex, 0, 594, 38, 34, 8, 17, 3, 0, 3, anim_speed, true);
	// Shealth Sword
	animator.create("SheathS", tex, 0, 631, 24, 39, 4, 0, 0, 0, 0, anim_speed, false);
	// Idle Shield
	animator.create("IdleShield", tex, 0, 670, 25, 30, 4, 10, 0, 0, 0, anim_speed / 4.5, true);
	// Walk F Shield
	animator.create("WalkRShield", tex, 0, 710, 23, 31, 8, 8, 0, 0, 0, anim_speed, true);
	// Walk R Shield
	animator.create("WalkFShield", tex, 0, 750, 23, 31, 8, 8, 0, 0, 0, anim_speed, true);
	// Sword Slash
	animator.create("Slash", tex, 0, 789, 62, 32, 7, 28, 0, 0, 0, anim_speed, false);
	// Dash
	animator.create("Dash", tex, 0, 823, 43, 17, 1, 25, -14, 0, -14, anim_speed, true);
	// Walk Combat
	animator.create("WalkC", tex, 0, 848, 25, 32, 8, 5, 1, 0, 1, anim_speed, true);


	Animator e_animator;
	//void create(std::string name, sf::Texture &t, int sx, int sy, int w, int h, int count, int ox, int oy, int rox, int roy, float spd, bool loop) {
	// Sword Chop
	e_animator.create("Chop", tex, 0, 9, 50, 31, 7, 15, 0, 0, 0, anim_speed, false);
	// Die
	e_animator.create("Die", tex, 0, 46, 50, 35, 14, 0, 2, 0, 2, anim_speed, false);
	// Dead
	e_animator.create("Dead", tex, 650, 46, 50, 35, 1, 0, 2, 0, 2, anim_speed, false);
	// Draw sword
	e_animator.create("DrawS", tex, 0, 86, 24, 34, 4, 11, 2, 0, 2, anim_speed, false);
	// Hit
	e_animator.create("Hit", tex, 0, 128, 26, 31, 1, 0, 0, 0, 0, anim_speed, true);
	// Idle Combat
	e_animator.create("Alert", tex, 0, 169, 20, 31, 3, 7, 0, 0, 0, anim_speed / 4.5, true);
	// Idle
	e_animator.create("Idle", tex, 0, 206, 13, 34, 3, 6, 2, 0, 2, anim_speed / 4.5, true);
	// Run Combat
	e_animator.create("RunC", tex, 0, 246, 38, 35, 8, 17, 3, 0, 3, anim_speed, true);
	// Shealth Sword
	e_animator.create("SheathS", tex, 0, 286, 24, 39, 4, 0, 0, 0, 0, anim_speed, false);
	// Stunned
	e_animator.create("Stunned", tex, 0, 320, 32, 30, 1, 15, -1, 0, -1, anim_speed, false);
	// Walk Combat
	e_animator.create("WalkC", tex, 0, 358, 25, 32, 8, 5, 1, 0, 1, anim_speed, true);

	sf::IntRect HpBlock, StmBlock, EBlock;
	HpBlock = sf::IntRect(0, 881, 4, 8);
	StmBlock = sf::IntRect(4, 881, 4, 8);
	EBlock = sf::IntRect(8, 881, 4, 8);
	float hud_x1, hud_y1;


	Player player(48, 128, animator, stage);
	Enemy enmey1(200, 128, e_animator, stage);
	Enemy enmey2(328, 128, e_animator, stage);
	Enemy enmey3(456, 128, e_animator, stage);

	std::vector<Actor*> actors;
	std::vector<Actor*>::iterator actors_it;

	actors.push_back(&player);
	actors.push_back(&enmey1);
	actors.push_back(&enmey2);
	actors.push_back(&enmey3);

	sf::Clock clock;

	float frames = 0;
	float ttl_time = 0;
	float av_time;
	float viewx = 128;
	float viewy = 128;

	float t_high = 0, t_low = 100;

	// GAME LOOP
	while (window.isOpen()) {
		// TIME MANAGEMENT
		float time = clock.restart().asMicroseconds();
		time = time / 10000; //Functions::Clamp(time / 1000, .19, 20);
		if (time >= t_high) {
			t_high = time;
		}

		if (time <= t_low) {
			t_low = time;
		}
		// AVERAGE TIME CALCULATION
		if (true) {
			frames++;
			av_time = (ttl_time + time) / frames;

			if (false) {
				sf::Text report;
				report.setFont(font);
				report.setCharacterSize(8);
				report.setPosition(16, 16);
				report.setFillColor(sf::Color::White);
				report.setString(
					std::to_string(av_time)
				);
				window.draw(report);
			}
		}
		//printf("%f\n", time);

		// 
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// INPUTS
		player.left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		player.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		player.up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		player.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		player.jump = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
		player.run = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
		player.attack = sf::Keyboard::isKeyPressed(sf::Keyboard::K);
		player.dash = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		player.shield_key = sf::Keyboard::isKeyPressed(sf::Keyboard::L);

		// PLAYER PROJECTILE COLLISIONS
		if (player.STATE == player.attacking) {
			if (player.currentFrame > 2 && player.currentFrame < 3) {
				std::vector<Actor*> at;

				switch (player.attack_stage) {
				case 1:	at = ActorTypeInRectangle(player.attack_collision_box_1, "Enemy", actors); break;
				case 2: at = ActorTypeInRectangle(player.attack_collision_box_2, "Enemy", actors); break;
				}

				for (actors_it = at.begin(); actors_it != at.end(); actors_it++) {
					Enemy *e = static_cast<Enemy*>(*actors_it);
					if (e->STATE != e->hit && e->STATE != e->die && e->STATE != e->dead) {
						e->STATE = e->hit;
						e->stun_timer = e->stun_timer_max;
						e->hit_dir = Functions::sgn(player.x - e->x);
						e->hp -= player.damage;
					}
				}
			}
		}

		// DRAW STUFF ON THE SCREEN
		window.clear(sf::Color::Black);

		if (player.x > 128) {
			viewx = player.x;
		}
		else {
			viewx = 128;
		}

		view.setCenter(viewx, viewy);
		window.setView(view);
		stage.Draw(window);

		for (actors_it = actors.begin(); actors_it != actors.end(); actors_it++) {
			Actor *a = *actors_it;
			if (a->type == "Enemy") {
				Enemy *e = static_cast<Enemy*>(a);
				e->player_x = player.x;
				e->player_y = player.y;

				// ENEMY ATTACK PROJECTILE COLLISION
				if (e->STATE == e->attack) {
					if (e->attack_collision_box.intersects(player.getCollisionRect())) {
						if (!player.invulnerable) {
							player.hit_dir = Functions::sgn(e->x - player.x);
							player.STATE = player.hit;
							player.stun_timer = player.stun_timer_max_shield;
							if (player.shield && player.hit_dir == player.facing) {
								e->STATE = e->stunned;
								e->stun_timer = e->stun_timer_max_hit_shield;
								e->attack_timer = e->attack_timer_max;
								player.stm -= 3;
							}
							else {
								player.take_damage = 1;
								player.hp -= e->damage;
								player.stun_timer = player.stun_timer_max;
							}
						}
					}
				}
			}
			a->Update(time);
			a->Draw(window, time);
		}

		// HUD
		{
			/*sf::Text HUD;
			HUD.setFont(font);
			HUD.setCharacterSize(8);
			HUD.setPosition(view.getCenter().x - (view.getSize().x/2) + 16, view.getViewport().top + 16);
			HUD.setFillColor(sf::Color::White);
			HUD.setString(std::to_string(t_low) + " <-" + std::to_string(av_time) + "-> " + std::to_string(t_high));
			window.draw(HUD);*/
		}

		hud_x1 = view.getCenter().x - (view.getSize().x / 2) + 16;
		hud_y1 = view.getViewport().top + 16;

		sf::Sprite block;
		block.setTexture(tex);
		for (int h = 0; h < player.hp_max; h++) {
			block.setPosition(hud_x1 + (5 * h), hud_y1);
			block.setTextureRect(HpBlock);

			if (h > player.hp - 1) {
				block.setTextureRect(EBlock);
			}
			window.draw(block);
		}

		for (int s = 0; s < player.stm_max; s++) {
			block.setPosition(hud_x1 + (5 * s), hud_y1 + 9);
			block.setTextureRect(StmBlock);
			if (s > player.stm - 1) {
				block.setTextureRect(EBlock);
			}
			window.draw(block);
		}


		window.display();
	}
};

std::vector<Actor*> Game::ActorTypeInRectangle(sf::FloatRect r, std::string t, std::vector<Actor*> &act) {
	std::vector<Actor*>::iterator it;
	std::vector<Actor*> ret_list;
	for (it = act.begin(); it != act.end(); it++) {
		Actor *a = *it;
		if (a->type == t) {
			if (r.intersects(a->getCollisionRect())) {
				ret_list.push_back(a);
			}
		}
	}

	return ret_list;
}