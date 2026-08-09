#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <optional>
#include "Level.hpp"
#include "LevelOne.hpp"
#include "LevelTwo.hpp"
#include <cmath>
#include "HUD.hpp"
#include "Interactive.hpp"

Game::Game(bool s)
{
	if (s != 0) status = 1;
	else status = s;
}

void Game::checkEvents_paused(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
	if (event->is<sf::Event::Closed>()) // closing the window by every possible way (but not from keyboard)
		window.close();

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
		{
			level = std::make_unique<LevelOne>(player);
			player.set_hp_to_default(); player.set_special_to_default();
		}
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
		if (keyPressed->scancode == sf::Keyboard::Scancode::P)
			pause();
		player.check_player_events(event, dt);

	}
	
}

void Game::draw(sf::RenderWindow& window)
{
	if (level != nullptr)
	{
		//LEVEL
		window.draw(*level);
		//PLAYER
		window.draw(player);
		//window.draw(player.get_debug_shape()); // debugging player hitbox
		window.draw(player.get_debug_2_shape()); //debugging player attackbox
		//HUD
		window.setView(window.getDefaultView());
		window.draw(hud);
	}
}

bool Game::get_status() const
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
		//PLAYER
		player.update_frame_status(dt);
		player.update_character_animation(dt);
}

void Game::position_update(sf::Time& dt)
{
	if (level != nullptr)
	{
		//PLAYER
		player.check_pressed();
		level->update_interactive_objects(dt, player.get_character_hitbox(), player.get_player_position(), player, level->get_tiles_in_row());
		player.apply_platform_velocity();
		player.character_position_update();
		player.check_player_collisions_with_interactive(level->get_tiles_in_row(), level->get_interactive_grid(), level->get_tiles_in_level(), level->get_character_grid());
		player.character_moving(dt, 375.f, 100.f, level->get_collision_array(), level->get_interactive_grid(), level->get_tiles_in_row(), level->get_tiles_in_level(), level->get_character_grid());
		player.check_velocity_y(dt, level->get_collision_array(), level->get_tiles_in_row(), level->get_interactive_grid(), level->get_tiles_in_level(), level->get_character_grid());
		player.check_velocity_x(dt, level->get_collision_array(), level->get_interactive_grid(), level->get_tiles_in_level(), level->get_tiles_in_row(), level->get_character_grid());
		player.character_position_update();
		if(player.get_pistol_mode()) player.pistol_mode_check(dt);
		if(player.get_attacked()) player.check_attacked(dt);
		else player.attack(dt);

		//INTERACITVE
		player.knocked_moving_latency(dt);


		//LEVEL
		level->set_camera_center(player.get_player_center());
		
		//HUD
		hud.hud_update(player.get_hp(), player.get_max_hp(), player.get_special_points(), player.get_max_special_points(), player.get_pistol_mode(), dt);
	}
}