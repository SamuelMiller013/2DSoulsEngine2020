#include "AnimationEngine.h"

Frame::Frame(int px, int py, int w, int h, int ox, int oy, int rox, int roy) {
	frame = sf::IntRect(px, py, w, h);
	frame_flip = sf::IntRect(px + w, py, -w, h);
	orx = ox;
	ory = oy;
	rorx = rox;
	rory = roy;
}