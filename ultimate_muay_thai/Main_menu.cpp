#include "Main_menu.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Interactive.hpp"
#include "Punching_bag.hpp"

Main_menu::Main_menu()
{
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
}

void Main_menu::check_buttons_activation_texture()
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

void Main_menu::update_menu_fighter_animation(sf::Time& dt)
{
	menu_fighter->menu_fighter_fighting(dt);
	menu_fighter->check_collision_with_punching_bag(punching_bag);
	menu_fighter->update_frame_status(dt);
	menu_fighter->update_character_animation(dt);
	punching_bag.texture_update(dt);
}

void Main_menu::button_enter(sf::RenderWindow& window)
{
	switch (active_button)
	{
	case 1:
		std::cout << "Start game button pressed\n";
		break;
	case 2:
		std::cout << "Settings button pressed\n";
		break;
	case 3:
		window.close();
		break;
	default:
		std::cout << "Start game button pressed\n";
	}
}

void Main_menu::button_up()
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

void Main_menu::button_down()
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

void Main_menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
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