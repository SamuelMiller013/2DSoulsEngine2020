#include "AnimationEngine.h"

Animation::Animation() {
	currentFrame = 0;
	isPlaying = true;
	flip = false;
}

float Animation::tick(float time) {
	if (!isPlaying) {
		return -1;
	}
	currentFrame += speed * time;
	if (frames.size() <= 0) {
		return -1;
	}

	if (currentFrame >= frames.size()) {
		currentFrame -= frames.size();
		if (!loop) {
			isPlaying = false;
			return -1;
		}
	}

	int i = currentFrame;
	sprite.setTextureRect(frames[i].frame);
	sprite.setOrigin(sf::Vector2f(frames[i].orx, frames[i].ory));
	if (flip) {
		sprite.setTextureRect(frames[i].frame_flip);
		sprite.setOrigin(sf::Vector2f(frames[i].rorx, frames[i].rory));
	}

	return currentFrame;
}