#pragma once
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "HUD.hpp"
#include <array>
#include <vector>
#include "Interactive.hpp"
#include "Game_status.hpp"
#include <SFML/Audio.hpp>
#include "Level.hpp"
#include "Main_menu.hpp"
#include "Game_status.hpp"

class Player : public Character
{
protected:

	
	//flags//////////////////////////////////////////////////////////////////////////////////////

	//boolean variable to check if the player is blocking or not
	bool attack_flag = { 0 };
	int attack_dir = { 0 }; //0 - middle, 1 high, 2 - low

	//SPECIAL
	short int special_points;
	short int max_special_points;

	short int lifes;
	const short int MAX_LIFES = { 3 };

	bool starting_strike = { 0 };

	bool is_shooting = { 0 };

	bool is_shooting_animation = { 0 };

	//moving_block
	bool moving_block;
	sf::Time time_moving_block;

	//
	bool pistol_mode = {false};
	sf::Time pistol_mode_time = { sf::seconds(0.f) };
	sf::Time shooting_latency = { sf::seconds(0.f) };

	sf::Time player_dying_time = { sf::seconds(0.f) };
	
	sf::Vector2f respawn_position = { 0,0 };

	bool player_dead = { false };
	bool player_game_over = { false };

	bool player_win = { false };

	//PISTOL_MODE_CUTSCENE
	sf::Texture devils_head_texture{ "textures/devils_head.png" };
	sf::Sprite devils_head_sprite{ devils_head_texture };
	float devils_head_difference = { 0.f };
	float DEVILS_HEAD_DIFFERENCE_MAX = { 10.f };
	bool devils_head_up = { true };
	bool cutscene_in_progress = { false };

	//AUDIO
		//pistol_mode cutscene
	sf::Music pistol_mode_cutscene_music{ "sound/effects/bulletproof.wav" };

public:
	friend class Level;
	friend class HUD;
	
	//constructor//////////////////////////////////////////////////////////////////////////////////////
	Player();

	//position
	void character_position_update();
	
	//animations//////////////////////////////////////////////////////////////////////////////////////
	void update_character_animation(sf::Time& dt) override; //setting the move animation of the character:
										//0 - 1 - default pose normal, 2-3 - default pose fight, 4 - block, 5-7 - moving normal, 8-9 - moving fight
	void update_frame_status(sf::Time& dt) override; //updating the status of the frame (for example, for a 2-frame animation, it will be 0 for the first frame and 1 for the second frame)

	void update_cutscenes(sf::Time& dt, Game_status& status, sf::View& camera);
	
	//checking events//////////////////////////////////////////////////////////////////////////////////////
	void check_player_events(const std::optional<sf::Event>& event, sf::Time& dt, Game_status& status); //checking events related to the player
	void check_pressed(); //checking pressed buttons
	void knocked_moving_latency(sf::Time& dt); //change knocked boolean
	void check_attacked(sf::Time& dt);
	void pistol_mode_check(sf::Time& dt);
	void check_hp(std::unique_ptr<Main_menu>& main_menu, Game_status& game_status, short int& current_level, std::unique_ptr<Level>& level);

	void respawn();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;



	//collisions//////////////////////////////////////////////////////////////////////////////////////
	void check_player_collisions_with_interactive(const int tiles_in_row, const std::vector<std::vector<Interactive*>>& interactive_grid, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid); //checking collisions of the player with interactive objects (for example, with a health pack)
	
	//getters//////////////////////////////////////////////////////////////////////////////////////
	bool get_is_fighting() const; //returning the fighting status of the player
	bool get_is_moving() const; //returning the moving status of the player
	bool get_is_blocking() const; //returning the blocking status of the player
	sf::Vector2f& get_player_position();
	sf::Vector2f get_player_center();
	const short int get_special_points();
	const short int get_max_special_points();
	const bool get_knocked();
	Interactive* get_standing_on_platform();
	const bool& get_starting_strike();
	const bool& get_pistol_mode() const;
	const bool& get_is_shooting() const;
	const sf::Vector2u& get_player_size() const;
	const bool& get_player_dead() const;
	const short int get_lifes() const;
	const bool& get_player_win() const;


	//setters//////////////////////////////////////////////////////////////////////////////////////
	void set_is_fighting(const bool status); //setting the fighting status of the player
	void set_is_moving(const bool status); //setting the moving status of the player
	void set_is_blocking(const bool status); //setting the blocking status of the player
	void set_starting_strike(const bool flag);
	void set_is_shooting(const bool flag);
	void set_hp_to_default();
	void set_special_to_default();
	void set_player_dead();
	void set_start_respawn(sf::Vector2f pos);
	void set_lifes_to_default();
	void set_player_win(bool w);

};