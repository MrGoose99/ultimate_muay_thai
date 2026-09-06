#include "Main_menu.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Interactive.hpp"
#include "Punching_bag.hpp"
#include "Game.hpp"
#include "Game_status.hpp"

Main_menu::Main_menu()
{
	menu_status = Menu_status::Main_menu;

	if (!background_texture.loadFromFile("menu/main_menu_background.png"))
		std::cout << "Error loading main menu background texture\n";
	if (!button_active_texture.loadFromFile("menu/button_active.png"))
		std::cout << "Error loading main menu button active texture\n";
	if (!button_unactive_texture.loadFromFile("menu/button_unactive.png"))
		std::cout << "Error loading main menu button unactive texture\n";
	if (!logo_texture.loadFromFile("menu/game_logo.png"))
		std::cout << "Error loading main menu logo texture\n";

	background_sprite.setTexture(background_texture);
	button_sprite_1.setTexture(button_active_texture);
	button_sprite_2.setTexture(button_unactive_texture);
	button_sprite_3.setTexture(button_unactive_texture);
	logo_sprite.setTexture(logo_texture);

	background_sprite.setPosition({ 0,0 });
	logo_sprite.setPosition({ 34, 33});
	button_sprite_1.setPosition({64,677});
	button_sprite_2.setPosition({64,812});
	button_sprite_3.setPosition({64,947});

	if (!pixeled_font.openFromFile("fonts/pixeled.ttf"))
		std::cout << "Error loading pixeled font\n";

	start_game_text.setFont(pixeled_font);
	start_game_text.setString("START GAME");
	start_game_text.setFillColor(sf::Color::Black);
	start_game_text.setCharacterSize(45);
	start_game_text.setPosition({ 244,710 });

	settings_text.setFont(pixeled_font);
	settings_text.setString("SETTINGS");
	settings_text.setFillColor(sf::Color::Black);
	settings_text.setCharacterSize(45);
	settings_text.setPosition({ 291,846 });

	exit_text.setFont(pixeled_font);
	exit_text.setString("EXIT");
	exit_text.setFillColor(sf::Color::Black);
	exit_text.setCharacterSize(45);
	exit_text.setPosition({ 381,981 });

	active_button = 1;

	punching_bag.set_position({ 1460,0 });
	punching_bag.set_scale({ 7.f, 7.86f});

	sf::Vector2f position = { 800, 300 };
	sf::Vector2f scale = { 6.f, 6.f };
	menu_fighter = std::make_unique<Menu_fighter>(position, scale);

	current_volume_value = 5;
	volume_value = 5;

	current_resolution = Resolution::_1920x1080;
	resolution = Resolution::_1920x1080;
}

void Main_menu::main_menu_set_up()
{
	background_sprite.setPosition({ 0,0 });
	logo_sprite.setPosition({ 34, 33 });
	button_sprite_1.setPosition({ 64,677 });
	button_sprite_2.setPosition({ 64,812 });
	button_sprite_3.setPosition({ 64,947 });

	start_game_text.setString("START GAME");
	start_game_text.setFillColor(sf::Color::Black);
	start_game_text.setCharacterSize(45);
	start_game_text.setPosition({ 244,710 });
;
	settings_text.setString("SETTINGS");
	settings_text.setFillColor(sf::Color::Black);
	settings_text.setCharacterSize(45);
	settings_text.setPosition({ 291,846 });

	exit_text.setString("EXIT");
	exit_text.setFillColor(sf::Color::Black);
	exit_text.setCharacterSize(45);
	exit_text.setPosition({ 381,981 });

	active_button = 1;

	punching_bag.set_position({ 1460,0 });
	punching_bag.set_scale({ 7.f, 7.86f });

	logo_sprite.setScale({ 1,1 });

	check_buttons_activation_texture();
}

void Main_menu::start_game_menu_set_up()
{
	start_game_menu_background_sprite.setPosition({ 460,240 });

	start_game_header_text.setFont(pixeled_font);
	start_game_header_text.setFillColor(sf::Color::Black);
	start_game_header_text.setString("CHOOSE LEVEL");
	start_game_header_text.setCharacterSize(50);
	start_game_header_text.setPosition({ 670, 277 });

	return_text.setFont(pixeled_font);
	return_text.setFillColor(sf::Color::Black);
	return_text.setString("RETURN");
	return_text.setCharacterSize(30);
	return_text.setPosition({ 876,770 });

	level_one_sprite.setPosition({ 870, 479 });

	check_buttons_activation_texture();
}

void Main_menu::settings_menu_set_up()
{
	settings_menu_background_sprite.setPosition({ 460,240 });

	settings_header_text.setFont(pixeled_font);
	settings_header_text.setFillColor(sf::Color::Black);
	settings_header_text.setString("SETTINGS");
	settings_header_text.setCharacterSize(50);
	settings_header_text.setPosition({ 765,277 });

	resolution_text.setFont(pixeled_font);
	resolution_text.setFillColor(sf::Color::Black);
	resolution_text.setString("RESOLUTION");
	resolution_text.setCharacterSize(40);
	resolution_text.setPosition({ 522, 454 });

	volume_text.setFont(pixeled_font);
	volume_text.setFillColor(sf::Color::Black);
	volume_text.setString("VOLUME");
	volume_text.setCharacterSize(40);
	volume_text.setPosition({ 585,580 });

	resolution_value_text.setFont(pixeled_font);
	resolution_value_text.setFillColor(sf::Color::Black);
	resolution_value_text.setString("1920x1080");
	resolution_value_text.setCharacterSize(40);
	sf::Vector2f resolution_origin = { resolution_value_text.getLocalBounds().size.x / 2, resolution_value_text.getLocalBounds().size.y / 2 };
	resolution_value_text.setOrigin(resolution_origin);
	resolution_value_text.setPosition({ 1210, 479 });


	apply_text.setFont(pixeled_font);
	apply_text.setFillColor(sf::Color::Black);
	apply_text.setString("APPLY");
	apply_text.setCharacterSize(30);
	apply_text.setPosition({ 1007, 770 });

	return_text.setFont(pixeled_font);
	return_text.setFillColor(sf::Color::Black);
	return_text.setString("RETURN");
	return_text.setCharacterSize(30);
	return_text.setPosition({742,770});

	float start_pos = 1060;
	for (auto& button : volume_buttons_sprites)
	{
		button.setPosition({ start_pos, 580 });
		button.setTextureRect(sf::IntRect(sf::Vector2i(15,0),sf::Vector2i(15,48)));
		start_pos += 30;
	}
	
	resolution = current_resolution;
	volume_value = current_volume_value;

	check_buttons_activation_texture();
}

void Main_menu::in_game_menu_set_up()
{
	in_game_menu_background_sprite.setPosition({ 710, 240 });

	logo_sprite.setScale({ 0.266f, 0.267f });
	logo_sprite.setPosition({ 847, 274 });

	shade.setFillColor(sf::Color(0, 0, 0, 50));
	shade.setSize({ 233, 161 });
	shade.setPosition({ 847, 274 });

	return_text.setFont(pixeled_font);
	return_text.setFillColor(sf::Color::Black);
	return_text.setString("RETURN");
	return_text.setCharacterSize(50);
	return_text.setPosition({ 805,471 });

	settings_text.setFont(pixeled_font);
	settings_text.setFillColor(sf::Color::Black);
	settings_text.setString("SETTINGS");
	settings_text.setCharacterSize(50);
	settings_text.setPosition({ 765, 576 });

	exit_text.setFont(pixeled_font);
	exit_text.setFillColor(sf::Color::Black);
	exit_text.setString("EXIT");
	exit_text.setCharacterSize(50);
	exit_text.setPosition({ 865,678 });

	check_buttons_activation_texture();
}

void Main_menu::check_buttons_activation_texture()
{
	if (menu_status == Menu_status::Main_menu)
	{
		switch (active_button)
		{
		case 1:
			button_sprite_1.setTexture(button_active_texture);
			button_sprite_2.setTexture(button_unactive_texture);
			button_sprite_3.setTexture(button_unactive_texture);
			break;
		case 2:
			button_sprite_1.setTexture(button_unactive_texture);
			button_sprite_2.setTexture(button_active_texture);
			button_sprite_3.setTexture(button_unactive_texture);
			break;
		case 3:
			button_sprite_1.setTexture(button_unactive_texture);
			button_sprite_2.setTexture(button_unactive_texture);
			button_sprite_3.setTexture(button_active_texture);
			break;
		default:
			button_sprite_1.setTexture(button_active_texture);
		}
	}
	else if (menu_status == Menu_status::Settings_menu || menu_status == Menu_status::In_game_settings_menu)
	{
		switch (active_button)
		{
		case 1:
			resolution_text.setFillColor(sf::Color(90, 92, 136));
			volume_text.setFillColor(sf::Color::Black);
			apply_text.setFillColor(sf::Color::Black);
			return_text.setFillColor(sf::Color::Black);
			break;
		case 2:
			resolution_text.setFillColor(sf::Color::Black);
			volume_text.setFillColor(sf::Color(90, 92, 136));
			apply_text.setFillColor(sf::Color::Black);
			return_text.setFillColor(sf::Color::Black);
			break;
		case 3:
			resolution_text.setFillColor(sf::Color::Black);
			volume_text.setFillColor(sf::Color::Black);
			apply_text.setFillColor(sf::Color(90, 92, 136));
			return_text.setFillColor(sf::Color::Black);
			break;
		case 4:
			resolution_text.setFillColor(sf::Color::Black);
			volume_text.setFillColor(sf::Color::Black);
			apply_text.setFillColor(sf::Color::Black);
			return_text.setFillColor(sf::Color(90, 92, 136));
			break;
		default:
			resolution_text.setFillColor(sf::Color(275, 73, 39));
			volume_text.setFillColor(sf::Color::Black);
			apply_text.setFillColor(sf::Color::Black);
			return_text.setFillColor(sf::Color::Black);
		}
		for (int i = 0; i < 10; i++)
		{
			if (i < volume_value)
				volume_buttons_sprites[i].setTextureRect(sf::IntRect(sf::Vector2i(15, 0), sf::Vector2i(15, 48)));
			else
				volume_buttons_sprites[i].setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(15, 48)));
		}

		switch (resolution)
		{
		case Resolution::_1920x1080:
			resolution_value_text.setString("1920x1080");
			break;
		case Resolution::_2560x1440:
			resolution_value_text.setString("2560x1440");
			break;
		case Resolution::_1280x720:
			resolution_value_text.setString("1280x720");
			break;
		default:
			resolution_value_text.setString("1920x1080");
		}
	}
	else if (menu_status == Menu_status::Start_game_menu)
	{
		switch (active_button)
		{
		case 1:
			level_one_sprite.setTexture(level_one_active_texture);
			return_text.setFillColor(sf::Color::Black);
			break;
		case 2:
			level_one_sprite.setTexture(level_one_unactive_texture);
			return_text.setFillColor(sf::Color(90, 92, 136));
			break;
		default:
			level_one_sprite.setTexture(level_one_active_texture);
			return_text.setFillColor(sf::Color::Black);
		}

	}
	else if (menu_status == Menu_status::In_game_menu)
	{
		switch (active_button)
		{
		case 1:
			return_text.setFillColor(sf::Color(90, 92, 136));
			settings_text.setFillColor(sf::Color::Black);
			exit_text.setFillColor(sf::Color::Black);
			break;
		case 2:
			return_text.setFillColor(sf::Color::Black);
			settings_text.setFillColor(sf::Color(90, 92, 136));
			exit_text.setFillColor(sf::Color::Black);
			break;
		case 3:
			return_text.setFillColor(sf::Color::Black);
			settings_text.setFillColor(sf::Color::Black);
			exit_text.setFillColor(sf::Color(90, 92, 136));
			break;
		default:
			return_text.setFillColor(sf::Color(90, 92, 136));
			settings_text.setFillColor(sf::Color::Black);
			exit_text.setFillColor(sf::Color::Black);
		}
	}
}

void Main_menu::update_menu_fighter_animation(sf::Time& dt)
{
	menu_fighter->menu_fighter_fighting(dt);
	menu_fighter->check_collision_with_punching_bag(punching_bag);
	menu_fighter->update_frame_status(dt);
	menu_fighter->update_character_animation(dt);
	punching_bag.texture_update(dt);
}

void Main_menu::button_enter(sf::RenderWindow& window, Game_status& status, short int& current_level)
{
	if (menu_status == Menu_status::Main_menu)
	{
		switch (active_button)
		{
		case 1:
			menu_status = Menu_status::Start_game_menu;
			active_button = 1;
			start_game_menu_set_up();
			break;
		case 2:
			menu_status = Menu_status::Settings_menu;
			active_button = 1;
			settings_menu_set_up();
			break;
		case 3:
			window.close();
			break;
		default:
			menu_status = Menu_status::Start_game_menu;
			active_button = 1;
			start_game_menu_set_up();
		}
	}
	else if (menu_status == Menu_status::Settings_menu || menu_status == Menu_status::In_game_settings_menu)
	{
		switch (active_button)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			if (status == Game_status::Main_menu)
				menu_status = Menu_status::Main_menu;
			else if (status == Game_status::In_game_menu)
			{
				menu_status = Menu_status::In_game_menu;
				active_button = 2;
				in_game_menu_set_up();
			}
			active_button = 2;
			switch (resolution)
			{
			case Resolution::_1920x1080:
				window.setSize({ 1920,1080 });;
				break;
			case Resolution::_2560x1440:;
				window.setSize({ 2560,1440});
				break;
			case Resolution::_1280x720:
				window.setSize({ 1280,720 });
				break;
			default:
				window.setSize({ 1920,1080 });
			}
			current_volume_value = volume_value;
			current_resolution = resolution;
			break;
		case 4:
			if (status == Game_status::Main_menu)
				menu_status = Menu_status::Main_menu;
			else if (status == Game_status::In_game_menu)
			{
				menu_status = Menu_status::In_game_menu;
				active_button = 2;
				in_game_menu_set_up();
			}
			active_button = 2;
			break;
		default:
			break;
		}
	}
	else if (menu_status == Menu_status::Start_game_menu)
	{
		switch (active_button)
		{
		case 1:
			menu_status = Menu_status::None;
			status = Game_status::Running;
			current_level = 1;
			break;
		case 2:
			menu_status = Menu_status::Main_menu;
			active_button = 1;
			break;
		default:
			menu_status = Menu_status::Main_menu;
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::In_game_menu)
	{
		switch (active_button)
		{
		case 1:
			menu_status = Menu_status::None;
			status = Game_status::Running;
			break;
		case 2:
			menu_status = Menu_status::In_game_settings_menu;
			active_button = 1;
			settings_menu_set_up();
			break;
		case 3:
			menu_status = Menu_status::Main_menu;
			status = Game_status::Main_menu;
			current_level = 0;
			main_menu_set_up();
			break;
		default:
			status = Game_status::Running;
		}
	}
}

void Main_menu::button_escape(sf::RenderWindow& window, Game_status& status, short int& current_level)
{
	switch (menu_status)
	{
	case Menu_status::In_game_menu:
	{
		menu_status = Menu_status::None;
		status = Game_status::Running;
		break;
	}
	case Menu_status::Settings_menu:
	{
		menu_status = Menu_status::Main_menu;
		active_button = 2;
		break;
	}
	case Menu_status::In_game_settings_menu:
	{
		menu_status = Menu_status::In_game_menu;
		active_button = 2;
		in_game_menu_set_up();
		break;
	}
	case Menu_status::Start_game_menu:
	{
		menu_status = Menu_status::Main_menu;
		active_button = 1;
		break;
	}
	case Menu_status::Main_menu:
	{
		break;
	}
	default:
	{
		menu_status = Menu_status::None;
		status = Game_status::Running;
		break;
	}
	}
}

void Main_menu::button_up()
{
	if (menu_status == Menu_status::Main_menu)
	{
		switch (active_button)
		{
		case 1:
			break;
		case 2:
			active_button = 1;
			break;
		case 3:
			active_button = 2;
			break;
		default:
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::Settings_menu || menu_status == Menu_status::In_game_settings_menu)
	{
		switch (active_button)
		{
		case 1:
			break;
		case 2:
			active_button = 1;
			break;
		case 3:
			active_button = 2;
			break;
		case 4:
			active_button = 2;
			break;
		default:
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::Start_game_menu)
	{
		switch (active_button)
		{
		case 1:
			break;
		case 2:
			active_button = 1;
			break;
		default:
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::In_game_menu)
	{
		switch (active_button)
		{
		case 1:
			break;
		case 2:
			active_button = 1;
			break;
		case 3:
			active_button = 2;
			break;
		default:
			active_button = 1;
		}
	}
	check_buttons_activation_texture();
}

void Main_menu::button_down()
{
	if (menu_status == Menu_status::Main_menu)
	{
		switch (active_button)
		{
		case 1:
			active_button = 2;
			break;
		case 2:
			active_button = 3;
			break;
		case 3:
			break;
		default:
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::Settings_menu || menu_status == Menu_status::In_game_settings_menu)
	{
		switch (active_button)
		{
		case 1:
			active_button = 2;
			break;
		case 2:
			active_button = 3;
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::Start_game_menu)
	{
		switch (active_button)
		{
		case 1:
			active_button = 2;
			break;
		case 2:
			break;
		default:
			active_button = 1;
		}
	}
	else if (menu_status == Menu_status::In_game_menu)
	{
		switch (active_button)
		{
		case 1:
			active_button = 2;
			break;
		case 2:
			active_button = 3;
			break;
		case 3:
			break;
		default:
			active_button = 1;
		}
	}
	check_buttons_activation_texture();
}

void Main_menu::button_left()
{
	if (menu_status == Menu_status::Settings_menu || menu_status == Menu_status::In_game_settings_menu)
	{
		switch (active_button)
		{
		case 1:
			switch (resolution)
			{
			case Resolution::_1920x1080:
				resolution = Resolution::_1280x720;
				break;
			case Resolution::_2560x1440:
				resolution = Resolution::_1920x1080;
				break;
			case Resolution::_1280x720:
				resolution = Resolution::_2560x1440;
				break;
			default:
				resolution = Resolution::_1920x1080;
			}
			break;
		case 2:
			if (volume_value > 0) volume_value--;
			break;
		case 3:
			active_button = 4;
			break;
		case 4:
			break;
		default:
			active_button = 1;
		}
	}
	check_buttons_activation_texture();
}

void Main_menu::button_right()
{
	if (menu_status == Menu_status::Settings_menu || menu_status == Menu_status::In_game_settings_menu)
	{
		switch (active_button)
		{
		case 1:
			switch (resolution)
			{
			case Resolution::_1920x1080:
				resolution = Resolution::_2560x1440;
				break;
			case Resolution::_2560x1440:
				resolution = Resolution::_1280x720;
				break;
			case Resolution::_1280x720:
				resolution = Resolution::_1920x1080;
				break;
			default:
				resolution = Resolution::_1920x1080;
			}
			break;
		case 2:
			if (volume_value < 10) volume_value++;
			break;
		case 3:
			break;
		case 4:
			active_button = 3;
			break;
		default:
			active_button = 1;
		}
	}
	check_buttons_activation_texture();
}

void Main_menu::button_in_game_menu()
{
	menu_status = Menu_status::In_game_menu;
	active_button = 1;
	in_game_menu_set_up();
}

void Main_menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	if (menu_status == Menu_status::Main_menu)
	{
		target.draw(background_sprite, states);

		target.draw(button_sprite_1, states);
		target.draw(button_sprite_2, states);
		target.draw(button_sprite_3, states);

		target.draw(start_game_text, states);
		target.draw(settings_text, states);
		target.draw(exit_text, states);

		target.draw(logo_sprite, states);

		target.draw(punching_bag, states);

		target.draw(*menu_fighter, states);

	}
	else if (menu_status == Menu_status::Settings_menu)
		{

		target.draw(background_sprite, states);

		target.draw(button_sprite_1, states);
		target.draw(button_sprite_2, states);
		target.draw(button_sprite_3, states);

		target.draw(start_game_text, states);
		target.draw(settings_text, states);
		target.draw(exit_text, states);

		target.draw(logo_sprite, states);

		target.draw(punching_bag, states);

		target.draw(*menu_fighter, states);

		target.draw(settings_menu_background_sprite, states);
		for (auto& button : volume_buttons_sprites)
			target.draw(button, states);

		target.draw(settings_header_text, states);
		target.draw(resolution_text, states);
		target.draw(resolution_value_text, states);
		target.draw(volume_text, states);
		target.draw(apply_text, states);
		target.draw(return_text, states);
		}
	
	else if (menu_status == Menu_status::Start_game_menu)
	{
		target.draw(background_sprite, states);

		target.draw(button_sprite_1, states);
		target.draw(button_sprite_2, states);
		target.draw(button_sprite_3, states);

		target.draw(start_game_text, states);
		target.draw(settings_text, states);
		target.draw(exit_text, states);

		target.draw(logo_sprite, states);

		target.draw(punching_bag, states);

		target.draw(*menu_fighter, states);

		target.draw(start_game_menu_background_sprite, states);
		target.draw(start_game_header_text, states);
		target.draw(level_one_sprite, states);
		target.draw(return_text, states);
	}
	else if (menu_status == Menu_status::In_game_menu)
	{
		target.draw(in_game_menu_background_sprite, states);
		target.draw(shade, states);
		target.draw(logo_sprite, states);
		target.draw(return_text, states);
		target.draw(settings_text, states);
		target.draw(exit_text, states);
	}
	else if (menu_status == Menu_status::In_game_settings_menu)
	{
		target.draw(settings_menu_background_sprite, states);
		for (auto& button : volume_buttons_sprites)
			target.draw(button, states);
		target.draw(settings_header_text, states);
		target.draw(resolution_text, states);
		target.draw(resolution_value_text, states);
		target.draw(volume_text, states);
		target.draw(apply_text, states);
		target.draw(return_text, states);
	}
}