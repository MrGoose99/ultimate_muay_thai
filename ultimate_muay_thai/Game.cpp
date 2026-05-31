#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <optional>
#include "Level.hpp"
#include "LevelOne.hpp"
#include "LevelTwo.hpp"
#include <cmath>

Game::Game(bool s)
{
	if (s != 0) status = 1;
	else status = s;
}

void Game::checkEvents_paused(const std::optional<sf::Event>& event)
{
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::P)
		{
			run();
		}
	}
}
void Game::checkEvents_running(const std::optional<sf::Event>& event, sf::RenderWindow& window, sf::Time& dt)
{
	if (event->is<sf::Event::Closed>()) // closing the window by every possible way (but not from keyboard)
		window.close();

	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
			level = std::make_unique<LevelOne>();
		if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
			level = std::make_unique<LevelTwo>();
		if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
			level->camera.move({ 10,0 });
		if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
			level->camera.move({ -10,0 });
		if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
			level->camera.move({ 0,10 });
		if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
			level->camera.move({ 0,-10 });
		if (keyPressed->scancode == sf::Keyboard::Scancode::NumpadMinus)
			level->camera.zoom(1.2f);
		if (keyPressed->scancode == sf::Keyboard::Scancode::NumpadPlus)
			level->camera.zoom(0.8f);
		player.check_player_events(event, dt);
	}
	
}

void Game::draw(sf::RenderWindow& window)
{
	if (level != nullptr)
	{
		//LEVEL
		window.setView(window.getDefaultView());
		window.draw(level->get_background());
		window.setView(level->camera);
		window.draw(level->get_tilemap());
		//PLAYER
		window.draw(player);
		//window.draw(player.get_debug_shape()); // debugging player hitbox
	}
}

bool Game::getStatus() const
{
	return status;
}

void Game::run()
{
	status = 1;
}

void Game::pause()
{
	status = 0;
}

void Game::animations_update(sf::Time& dt)
{
	if(level != nullptr)
		player.update_frame_status(dt);
		player.update_character_animation(dt);
		player.check_pressed(); 
}

void Game::position_update(sf::Time& dt)
{
	if (level != nullptr)
	{
		//PLAYER
		player.character_moving(dt, 375.f, 100.f, level->get_collision_array());
		player.check_velocity(dt, level->get_collision_array());
		player.character_position_update();

		//LEVEL
		level->set_camera_center(player.get_player_center());
	}
}