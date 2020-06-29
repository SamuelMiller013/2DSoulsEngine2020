#include "Actor.h"

Actor::Actor(int px, int py, Animator a, Stage s) {
	x = px;
	y = py;
	wid = 16;
	hig = 16;
	obj = s.GetAllObjects();
	invulnerable = 0;
	animator = a;
}


sf::FloatRect Actor::getCollisionRect() {
	return sf::FloatRect(x, y, wid, hig);
}

void Actor::SetCollisionRect(int px, int py, int pwid, int phig) {
	x = px;
	y = py;
	wid = pwid;
	hig = phig;
	center_x = x + wid / 2;
	center_y = y + hig / 2;
};

void Actor::ReactiveCollision(int num) {
	for (int i = 0; i < obj.size(); i++) {
		if (getCollisionRect().intersects(obj[i].rect) && obj[i].type == "Solid") {
			if (num == 0) {
				if (y > obj[i].rect.top) { // head
					y = obj[i].rect.top + obj[i].rect.height;
					vspd = 0;
				}
				else if (y < obj[i].rect.top) { // feet
					y = obj[i].rect.top - hig;
					vspd = 0;
				}
			}
			if (num == 1) {
				if (x > obj[i].rect.left) { // moving left
					x = obj[i].rect.left + obj[i].rect.width;
					hspd = 0;
				}
				else if (x < obj[i].rect.left) { // moving right
					x = obj[i].rect.left - wid;
					hspd = 0;
				}
			}
		}
	}
}