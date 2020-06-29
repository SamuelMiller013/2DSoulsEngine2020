#include "Level.h"

std::vector<Object> Stage::GetAllObjects()
{
	return objects;
};

void Stage::Draw(sf::RenderWindow &window) {
	for (int tile = 0; tile < layers[0].tiles.size(); tile++) {
		sf::Sprite sprite = layers[0].tiles[tile];
		window.draw(sprite);
	}

	if (false) {
		for (int o = 0; o < objects.size(); o++) {
			sf::RectangleShape rec(sf::Vector2f(objects[o].rect.width, objects[o].rect.height));
			rec.setPosition(sf::Vector2f(objects[o].rect.left, objects[o].rect.top));
			if (objects[o].type == "Solid") {
				rec.setFillColor(sf::Color::Blue);
			}
			else if (objects[o].type == "Stairs") {
				rec.setFillColor(sf::Color::Yellow);
			}
			window.draw(rec);
		}
	}
}

bool Stage::buildLevelFromTMX(std::string filename) {
	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLError eResult = xmlDoc.LoadFile(filename.c_str());

	tinyxml2::XMLElement * map_el = xmlDoc.FirstChildElement();
	if (map_el == nullptr) {
		std::cout << "map_el doesn't exist";
		return false;
	}

	int grid_width, grid_height, cellsize;

	eResult = map_el->QueryIntAttribute("width", &grid_width);
	eResult = map_el->QueryIntAttribute("height", &grid_height);
	eResult = map_el->QueryIntAttribute("tilewidth", &cellsize);

	tinyxml2::XMLElement * tileset_el = map_el->FirstChildElement("tileset");
	if (tileset_el == nullptr) {
		std::cout << "no tileset found " << tileset_el;
		return false;
	}

	// GET THE TILE MAP FOR THE BRICKS ============================================================
	sf::Image img;
	if (!img.loadFromFile("images/tiles.png")) {
		std::cout << "Failed to load tile sheet." << std::endl;
		return false;
	}

	img.createMaskFromColor(sf::Color(255, 255, 255));
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);

	// BUILD OUT LIST OF TILE RECTS ====================================================================
	// Get the number of columns and rows
	int columns = tilesetImage.getSize().x / cellsize;
	int rows = tilesetImage.getSize().y / cellsize;

	// Get a list of vector images from individual rectangles (TextureRect)
	std::vector<sf::Rect<int> > subRects;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * cellsize;
			rect.height = cellsize;
			rect.left = x * cellsize;
			rect.width = cellsize;

			subRects.push_back(rect);
		}
	}

	// BUILD OUT MAP LAYERS ======================================================================
	Layer layer;
	layer.opacity = 255;

	tinyxml2::XMLElement * layer_el = map_el->FirstChildElement("layer");
	if (layer_el == nullptr) {
		std::cout << "no image found " << layer_el;
		return false;
	}
	tinyxml2::XMLElement * data_el = layer_el->FirstChildElement("data");
	if (data_el == nullptr) {
		std::cout << "no image found " << data_el;
		return false;
	}

	const char* csv = data_el->GetText();
	//std::cout << csv;

	int size = grid_width * grid_height;
	int index = 0;
	int value = 0;
	int x = 0;
	int y = 0;

	while (true) {
		std::string cell_val = "";
		while (true) {
			char tmp = csv[index];
			index++;
			if (tmp == ',' || value >= size) {
				value++;
				break;
			}
			else if (tmp != '\n') {
				cell_val += tmp;
			}
		}

		int tileGID = stoi(cell_val, nullptr, 10);
		if (tileGID != 0) {
			tileGID--;
		}
		if (tileGID > subRects.size()) {
			std::cout << "index out of bounds " << tileGID << std::endl;
			return false;
		}

		float xx = x * cellsize;
		float yy = (y * cellsize);// +48;

		sf::Sprite sprite;
		sprite.setTexture(tilesetImage);
		sprite.setTextureRect(subRects[tileGID]);
		sprite.setPosition(xx, yy);
		sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
		layer.tiles.push_back(sprite);

		x += 1;
		if (x >= grid_width) {
			x = 0;
			y++;
			if (y >= grid_height) {
				y = 0;
			}
		}

		if (value >= size) {
			break;
		}
	}

	layers.push_back(layer);


	tinyxml2::XMLElement *objectGroupElement;

	if (map_el->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map_el->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			tinyxml2::XMLElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				std::string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - 1].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - 1].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - 1]);
				}

				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = height;
				objectRect.width = width;
				object.rect = objectRect;


				tinyxml2::XMLElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					tinyxml2::XMLElement *prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}