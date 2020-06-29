#pragma once
#include "stdafx.h"

class Frame {
public:
	Frame(int px, int py, int w, int h, int ox, int oy, int rox, int roy);

	int orx, ory, rorx, rory;
	sf::IntRect frame, frame_flip;
};






class Animation {
public:
	std::vector<Frame> frames;
	sf::Sprite sprite;
	float speed, currentFrame;
	bool flip, isPlaying, loop;

	Animation();

	float tick(float time);
};





class Animator {
public:
	std::map<std::string, Animation> anim_list;
	std::string currentAnim;

	Animator();

	void create(std::string name, sf::Texture &t, std::vector<Frame> &fra, float spd, bool loop);

	void create(std::string name, sf::Texture &t, int sx, int sy, int w, int h, int count, int ox, int oy, int rox, int roy, float spd, bool loop);

	void set(std::string name);

	float draw(sf::RenderWindow &window, float time, int x, int y);

	void flip(bool b = 1);

	bool GetIsPlaying();

	void SetIsPlaying();

	int GetFrame();
};
