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
#include "Game_status.hpp"

Game::Game()
{
	status = Game_status::Main_menu;
	main_menu = std::make_unique<Main_menu>();
	current_level = 0;
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
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
		{
			status = Game_status::In_game_menu;
			main_menu->button_in_game_menu();
			return;
		}
		if(!player.get_player_dead())player.check_player_events(event, dt, status);

	}
	
}

void Game::checkEvents_main_menu(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
	if (event->is<sf::Event::Closed>()) // closing the window by every possible way (but not from keyboard)
		window.close();
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Up || keyPressed->scancode == sf::Keyboard::Scancode::W) //BUTTON UP
		{
			main_menu->button_up();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Down || keyPressed->scancode == sf::Keyboard::Scancode::S) //BUTTON DOWN
		{
			main_menu->button_down();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Left || keyPressed->scancode == sf::Keyboard::Scancode::A) //BUTTON LEFT
		{
			main_menu->button_left();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Right || keyPressed->scancode == sf::Keyboard::Scancode::D) //BUTTON RIGHT
		{
			main_menu->button_right();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode == sf::Keyboard::Scancode::Space) //ENTER
		{
			main_menu->button_enter(window, status, current_level);
			if (current_level == 1)
			{
				level = std::make_unique<LevelOne>(player);
				player.set_hp_to_default(); player.set_special_to_default();
			}
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
		{
			main_menu->button_escape(window, status, current_level);
		}
	}
}

void Game::checkEvents_in_game_menu(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{

	if (event->is<sf::Event::Closed>()) // closing the window by every possible way (but not from keyboard)
		window.close();
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Up || keyPressed->scancode == sf::Keyboard::Scancode::W) //BUTTON UP
		{
			main_menu->button_up();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Down || keyPressed->scancode == sf::Keyboard::Scancode::S) //BUTTON DOWN
		{
			main_menu->button_down();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Left || keyPressed->scancode == sf::Keyboard::Scancode::A) //BUTTON LEFT
		{
			main_menu->button_left();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Right || keyPressed->scancode == sf::Keyboard::Scancode::D) //BUTTON RIGHT
		{
			main_menu->button_right();
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode == sf::Keyboard::Scancode::Space) //ENTER
		{
			main_menu->button_enter(window, status, current_level);
			if (current_level == 0)
			{
				status = Game_status::Main_menu;
				current_level = 0;
				level = nullptr;
			}
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
		{
			main_menu->button_escape(window, status, current_level);
		}
	}
}

void Game::draw(sf::RenderWindow& window)
{
	if (status == Game_status::Main_menu)
	{
		window.setView(window.getDefaultView());
		window.draw(*main_menu);
	}
	else
	{
		if (level != nullptr)
		{
			//LEVEL
			window.draw(*level);
			//PLAYER
			window.draw(player);
			//window.draw(player.get_debug_shape()); // debugging player hitbox
			//window.draw(player.get_debug_2_shape()); //debugging player attackbox
			//HUD
			window.setView(window.getDefaultView());
			window.draw(hud);
		}
		if (status == Game_status::In_game_menu || status == Game_status::Game_over)
		{
			window.draw(*main_menu);
		}
	}

}

Game_status Game::get_status() const
{
	return status;
}

void Game::run()
{
	status = Game_status::Running;
}

void Game::pause()
{
	status = Game_status::Paused;
}

void Game::animations_update(sf::Time& dt)
{
	if (level != nullptr)
	{
		//PLAYER
		if (status == Game_status::Running || status == Game_status::Game_over)
		{
			player.update_frame_status(dt);
			player.update_character_animation(dt);
		}
		else if (status == Game_status::Cutscene)
		player.update_cutscenes(dt, status, level->camera);
	}
}

void Game::position_update(sf::Time& dt)
{
	if (level != nullptr)
	{
		//PLAYER
		if (status == Game_status::Running || status == Game_status::Game_over)
		{

			player.check_pressed();
			level->update_interactive_objects(dt, player.get_character_hitbox(), player.get_player_position(), player, level->get_tiles_in_row());
			player.apply_platform_velocity();
			player.character_position_update();
			player.check_player_collisions_with_interactive(level->get_tiles_in_row(), level->get_interactive_grid(), level->get_tiles_in_level(), level->get_character_grid());
			if (!player.get_player_dead())player.character_moving(dt, 375.f, 100.f, level->get_collision_array(), level->get_interactive_grid(), level->get_tiles_in_row(), level->get_tiles_in_level(), level->get_character_grid());
			player.check_velocity_y(dt, level->get_collision_array(), level->get_tiles_in_row(), level->get_interactive_grid(), level->get_tiles_in_level(), level->get_character_grid());
			player.check_velocity_x(dt, level->get_collision_array(), level->get_interactive_grid(), level->get_tiles_in_level(), level->get_tiles_in_row(), level->get_character_grid());
			player.character_position_update();
			if (player.get_pistol_mode() && !player.get_player_dead()) player.pistol_mode_check(dt);
			if (player.get_attacked() && !player.get_player_dead()) player.check_attacked(dt);
			else player.attack(dt);
			player.check_hp(main_menu, status, current_level, level);

			if (player.get_player_win())
			{
				you_win_time += dt;
				status = Game_status::Game_over;
				main_menu->you_win_menu_set_up();
				main_menu->set_menu_status(Main_menu::Menu_status::You_win_menu);
				if (you_win_time >= sf::seconds(3.5f))
				{
					level = nullptr;
					current_level = 0;
					main_menu->main_menu_set_up();
					status = Game_status::Main_menu();
					main_menu->set_menu_status(Main_menu::Menu_status::Main_menu);
					player.set_hp_to_default(); player.set_lifes_to_default(); player.set_special_to_default();
					player.set_player_win(false);
					you_win_time = sf::seconds(0.f);
				}
			}
			else you_win_time = sf::seconds(0.f);
			
		}
		else if (status == Game_status::Cutscene)
		{
			player.character_position_update();
		}

		if (level != nullptr)
		{
			//INTERACITVE
			player.knocked_moving_latency(dt);

			//LEVEL
			level->set_camera_center(player.get_player_center());

			//HUD
			hud.hud_update(player.get_hp(), player.get_max_hp(), player.get_special_points(), player.get_max_special_points(), player.get_pistol_mode(), dt, player.get_lifes());
		}

	}
	else if (status == Game_status::Main_menu)
	{
		main_menu->main_menu_sound();
		main_menu->update_menu_fighter_animation(dt);
	}
}