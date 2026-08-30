#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"
#include "Punching_bag.hpp"
#include "Menu_fighter.hpp"
#include "Character.hpp"

class Main_menu : public sf::Drawable, public sf::Transformable
{
protected:
	sf::Texture background_texture{"menu/main_menu_background.png"};
	sf::Texture button_active_texture{"menu/button_active.png"};
	sf::Texture button_unactive_texture{"menu/button_unactive.png"};
	sf::Texture logo_texture{"menu/game_logo.png"};

	sf::Sprite background_sprite{ background_texture };
	sf::Sprite button_sprite_1{ button_unactive_texture };
	sf::Sprite button_sprite_2{ button_unactive_texture };
	sf::Sprite button_sprite_3{ button_unactive_texture };
	sf::Sprite logo_sprite{ logo_texture };
	
	sf::Font pixeled_font;

	sf::Text start_game_text{ pixeled_font };
	sf::Text settings_text{ pixeled_font };
	sf::Text exit_text{ pixeled_font };

	Punching_bag punching_bag{ 1,1 };

	std::unique_ptr<Character> menu_fighter;

	int active_button;

public:
	Main_menu();
	void button_enter(sf::RenderWindow& window);
	void check_buttons_activation_texture();
	void button_up();
	void button_down();
	void update_menu_fighter_animation(sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


};