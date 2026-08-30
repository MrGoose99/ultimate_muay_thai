#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Level.hpp"
#include "LevelOne.hpp"
#include "LevelTwo.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "HUD.hpp"
#include "Interactive.hpp"
#include "Main_menu.hpp"

class Game
{
	int status; // 1 for running, 0 for paused, 2 for main menu
	std::unique_ptr<Level> level; //pointer to the current level
	HUD hud; //HUD of the game (at the moment same for every lvl)
	sf::Texture background; //background of the level
	sf::RectangleShape background_shape; //shape of the background of the level
	Player player; //player character
	std::unique_ptr<Main_menu> main_menu; //main menu of the game

public:
	Game();
	void checkEvents_paused(const std::optional<sf::Event>& event, sf::RenderWindow& window); //checking for events when the game is paused
	void checkEvents_running(const std::optional<sf::Event>& event, sf::RenderWindow& window, sf::Time& dt); //checking for events when the game is running
	void checkEvents_main_menu(const std::optional<sf::Event>& event, sf::RenderWindow& window); //checking for events when the game is in main menu
	int get_status() const; //returning the status of the game
	void draw(sf::RenderWindow& window); //drawing the game
	void run(); //running the game
	void pause(); //pausing the game
	void animations_update(sf::Time& dt); //updating the animations of the characters
	void position_update(sf::Time& dt); //updating elements positions
};