#pragma once
#include "SFML/Graphics.hpp"
#include "const.h"
class Player {
private:
	sf::Texture texture;
	sf::Sprite sprite;
	int hp = PLAYER_HP;
	bool del = false;
public:
	Player(float x, float y) {
		texture.loadFromFile(IMAGES_FOLDER + SHIP_FILE_NAME);
		sprite.setTexture(texture);
		sf::FloatRect g_bounds = sprite.getGlobalBounds();
		//sprite.setPosition(WINDOW_WIDTH / 2 - 50,
		//	WINDOW_HEIGHT - 150);
		init(x, y);
	}
	void init(float x, float y) {
		sprite.setPosition(x, y);
	}
	
int getHp();
	
	void update();
	void draw(sf::RenderWindow& window);
	sf::FloatRect getHitBox();
	sf::Vector2f getPosition();
	void setDel(bool);
	bool getDel();
	void decreaseHp(size_t);
	void increaseHp(size_t);
	
};