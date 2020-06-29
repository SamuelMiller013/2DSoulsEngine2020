#pragma once
#include "stdafx.h"
#include "tinyxml2.h"

struct Object
{
	int GetPropertyInt(std::string name);
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;
	std::string type;
	sf::Rect<float> rect;
	std::map<std::string, std::string> properties;

	sf::Sprite sprite;
};

struct Layer
{
	int opacity;
	std::vector<sf::Sprite> tiles;
};

class Stage {
public:
	std::vector<Object> GetAllObjects();

	void Draw(sf::RenderWindow &window);

	bool buildLevelFromTMX(std::string filename);

private:
	std::vector<Object> objects;
	std::vector<Layer> layers;
	sf::Texture tilesetImage;
};