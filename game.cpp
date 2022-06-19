#include "game.h"
#include "const.h"
#include <iostream>
Game::Game() :
	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close),
	game_over_screen(IMAGES_FOLDER + GAME_OVER_FILE_NAME, 3.f, 3.f),
	player_hp(TEXT_FILE_NAME, 18, 20, 10, sf::Color::Black),
	score_text(TEXT_FILE_NAME, 18, 520, 10, sf::Color::Black),
	intro_screen(IMAGES_FOLDER + INTRO_FILE_NAME, 0.70f, 0.80f),
	tim_text(TEXT_FILE_NAME, 30, 100, 600, sf::Color::Black),
	river1(0.f, 0.f),
	river2(0.f, -WINDOW_HEIGHT),
	player(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 150)
{

	window.setVerticalSyncEnabled(true);


	for (size_t i = 0; i < BARIER_QTY; i++) {
		Barier* barier = new Barier();
		bariers.push_back(barier);
	}
	int ft = 0;
	for (size_t i = 0; i < ISLANDS_TYPE_QTY; i++) {
		Islands* islands = new Islands();
		island.push_back(islands);
		player_lives = player_lives + 2;
	}
}
void Game::init() {
	player.init(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 150);
	river1.init(0.f, 0.f);
	river2.init(0.f, -WINDOW_HEIGHT);
}
void Game::play() {

	//init();

	while (window.isOpen()) {
		check_events();
		update();
		check_collisions();
		draw();
	}
}
void Game::check_events() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();
		if (game_state == GAME_OVER && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y) {
			game_state = INTRO;
			player_lives = 5;
		}
		else if (game_state == GAME_OVER && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N) {
			game_state = GAME_OVER;
			exit(0);
		}
		if (event.type == sf::Event::KeyPressed) {
			if (game_state == INTRO && event.key.code == sf::Keyboard::Space) {
				game_state = PLAY;
			}
		}
	}
}
void Game::update() {
	switch (game_state) {
	case INTRO:
		break;
	case PLAY:
		river1.update();
		river2.update();
		player.update();
		player_hp.update(std::to_string(player.getHp()));

		score_text.update("Score: " + std::to_string(score));
		if (player.getHp() == 0) game_state = GAME_OVER;
		for (size_t i = 0; i < BARIER_QTY; i++) {
			bariers[i]->update();
		}
		if (player.getHp() <= 0) game_state = GAME_OVER;
		//for (auto iter = explosions.begin(); iter != explosions.end(); iter++) {
		//	(*iter)->update();
		//}
		for (auto iter = bonuses.begin(); iter != bonuses.end(); iter++) {
			(*iter)->update();
		}
		for (size_t i = 0; i < ISLANDS_TYPE_QTY; i++) {
			island[i]->update();
		}
		break;
	case GAME_OVER:
		tim_text.update(QUESTEN);
		break;
	}
}
void Game::draw() {
	window.clear(sf::Color::Black);
	switch (game_state) {
	case INTRO:
		window.draw(game_over_screen.getSprite());
		break;
	case PLAY:
		river2.draw(window);
		river1.draw(window);
		player.draw(window);
		for (size_t i = 0; i < BARIER_QTY; i++) {
			bariers[i]->draw(window);
		};
		for (size_t i = 0; i < ISLANDS_TYPE_QTY; i++) {
			island[i]->draw(window);
		};
		window.draw(player_hp.getText());
		//for (auto iter = explosions.begin(); iter != explosions.end(); iter++) {
		//	(*iter)->draw(window);
		//}
		for (auto iter = bonuses.begin(); iter != bonuses.end(); iter++) {
			(*iter)->draw(window);
		}
		window.draw(score_text.getText());
		break;
	case GAME_OVER:
		window.clear(sf::Color(255, 222, 173));
		window.draw(intro_screen.getSprite());
		window.draw(tim_text.getText());
		break;
	}
	window.display();
}
void Game::check_collisions() {
	for (auto iter = bariers.begin(); iter != bariers.end(); iter++) {
		if (player.getHitBox().intersects((*iter)->getHitBox())) {
			player_lives = player_lives - 1;
			player.decreaseHp(static_cast<size_t>(player_lives));

			(*iter)->spawn();
			if (player_lives == 0) {
				game_state = GAME_OVER;
			}
		}
	}
	sf::Event event;
	window.pollEvent(event);
	for (auto ite_r = island.begin(); ite_r != island.end(); ite_r++) {
		if (player.getHitBox().intersects((*ite_r)->getHitBox())) {
			(*ite_r)->spawn();
			game_state = GAME_OVER;
		}
	}
	for (auto it_l = bariers.begin(); it_l != bariers.end(); it_l++) {
		for (auto it_m = players.begin(); it_m != players.end(); it_m++) {
			if ((*it_l)->getHitBox().intersects((*it_m)->getHitBox())) {
				score += 80 - static_cast<size_t>((*it_m)->getHitBox().width);/// 2);
				//Explosion* new_exp = new Explosion((*it_m)->getPosition());
				//explosions.push_back(new_exp);
				Bonus* new_bonus = new Bonus((*it_m)->getPosition());
				bonuses.push_back(new_bonus);
			}
		}
	}
	for (auto iter = bonuses.begin(); iter != bonuses.end(); iter++) {
		if (player.getHitBox().intersects((*iter)->getHitBox())) {
			(*iter)->setDel(true);
			switch ((*iter)->getType()) {
			case Bonus::BonusType::PILL:
				player.increaseHp(5);
				break;
			}
		}
	}
	//explosions.remove_if([](Explosion* exp) {return exp->getDel(); });
}