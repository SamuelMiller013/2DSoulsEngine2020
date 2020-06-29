#include "AnimationEngine.h"

Animator::Animator() {};

void Animator::create(std::string name, sf::Texture &t, std::vector<Frame> &fra, float spd, bool loop) {
	Animation a;
	a.sprite.setTexture(t);
	a.frames = fra;
	a.speed = spd;
	a.loop = loop;
	anim_list[name] = a;
	currentAnim = name;
}

void Animator::create(std::string name, sf::Texture &t, int sx, int sy, int w, int h, int count, int ox, int oy, int rox, int roy, float spd, bool loop) {
	Animation a;
	a.sprite.setTexture(t);
	a.speed = spd;
	a.loop = loop;

	for (int i = 0; i < count; i++) {
		a.frames.push_back(Frame(sx + (i*w), sy, w, h, ox, oy, w - ox, roy));
	}

	anim_list[name] = a;
	currentAnim = name;
}

void Animator::set(std::string name) {
	currentAnim = name;
	anim_list[currentAnim].flip = 0;
}

float Animator::draw(sf::RenderWindow &window, float time, int x, int y) {
	float f = anim_list[currentAnim].tick(time);
	anim_list[currentAnim].sprite.setPosition(x, y);
	window.draw(anim_list[currentAnim].sprite);

	return f;
}

void Animator::flip(bool b) {
	anim_list[currentAnim].flip = b;
}

bool Animator::GetIsPlaying() {
	return anim_list[currentAnim].isPlaying;
}

void Animator::SetIsPlaying() {
	anim_list[currentAnim].isPlaying = true;
}

int Animator::GetFrame() {
	return anim_list[currentAnim].currentFrame;
}