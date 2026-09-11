#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"
#include "Punching_bag.hpp"
#include "Menu_fighter.hpp"
#include "Character.hpp"
#include "Game_status.hpp"
#include <SFML/Audio.hpp>

class Game;
class Main_menu : public sf::Drawable, public sf::Transformable
{
protected:
	//Main_menu
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

	//Settings_menu
	sf::Texture context_menu_background_texture{ "menu/context_menu_background.png" };
	sf::Texture volume_button_texture{ "menu/volume_button.png" };

	sf::Sprite settings_menu_background_sprite{ context_menu_background_texture };

	sf::Sprite volume_buttons_sprites[10] = 
	{ sf::Sprite { volume_button_texture},
	sf::Sprite { volume_button_texture },
	sf::Sprite{ volume_button_texture },
	sf::Sprite { volume_button_texture },
	sf::Sprite { volume_button_texture },
	sf::Sprite { volume_button_texture },
	sf::Sprite { volume_button_texture },
	sf::Sprite { volume_button_texture },
	sf::Sprite { volume_button_texture },
	sf::Sprite { volume_button_texture }
	};

	sf::Text settings_header_text{ pixeled_font };
	sf::Text resolution_text{ pixeled_font };
	sf::Text resolution_value_text{ pixeled_font };
	sf::Text volume_text{ pixeled_font };
	sf::Text apply_text{ pixeled_font };
	sf::Text return_text{ pixeled_font }; //also for start_game menu

	//START_GAME MENU
	sf::Texture level_one_unactive_texture{ "menu/level_one_unactive.png" };
	sf::Texture level_one_active_texture{ "menu/level_one_active.png" };

	sf::Sprite start_game_menu_background_sprite{ context_menu_background_texture };
	sf::Sprite level_one_sprite{ level_one_unactive_texture };

	sf::Text start_game_header_text{ pixeled_font };

	//IN_GAME_MENU
	sf::RectangleShape shade;

	sf::Texture in_game_menu_background_texture{ "menu/in_game_menu_background.png" };

	sf::Sprite in_game_menu_background_sprite{ in_game_menu_background_texture };

	//GAME_OVER
	sf::Text game_over_text{ pixeled_font };
	sf::Text game_over_shadow_text{ pixeled_font };

	//YOU_WIN
	sf::Text you_win_text{ pixeled_font };
	sf::Text you_win_shadow_text{ pixeled_font };

	short int current_volume_value;
	short int volume_value;

	short int active_button;

public:
	enum class Menu_status
	{
		None,
		Main_menu,
		Settings_menu,
		Start_game_menu,
		In_game_menu,
		In_game_settings_menu,
		Game_over_menu,
		You_win_menu
	};
protected:
	enum class Resolution
	{
		_1920x1080,
		_2560x1440,
		_1280x720
	};

	Menu_status menu_status;

	Resolution resolution;
	Resolution current_resolution;

	//SOUND
	sf::Music main_menu_music{ "sound/music/main_menu_music.wav" };
	bool music_playing = { false };

public:
	Main_menu();
	void button_enter(sf::RenderWindow& window, Game_status& status, short int& current_level);
	void button_escape(sf::RenderWindow& window, Game_status& status, short int& current_level);
	void check_buttons_activation_texture();
	void button_up();
	void button_down();
	void button_left();
	void button_right();
	void button_in_game_menu();
	void main_menu_set_up();
	void start_game_menu_set_up();
	void settings_menu_set_up();
	void in_game_menu_set_up();
	void game_over_menu_set_up();
	void update_menu_fighter_animation(sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const Menu_status& get_menu_status() const;

	void set_menu_status(const Menu_status& status);
	void you_win_menu_set_up();

	void main_menu_sound();


};